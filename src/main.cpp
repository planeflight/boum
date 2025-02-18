#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <cmath>
#include <cstring>
#include <iostream>

#include "math.hpp"
#include "util/types.hpp"
#include "video.hpp"

static constexpr i32 window_width = 1600, window_height = 900;

static constexpr u32 num_frames = 1500;
f32 music_frames[num_frames * 2] = {0.0f};
u32 current_frames = 0;

void wave(float *samples, u32 frames) {
    // if overflow
    if (current_frames + frames >= num_frames) {
        std::memmove(music_frames,
                     music_frames + (frames) * 2,
                     sizeof(f32) * (num_frames - frames) * 2);
        current_frames = num_frames - frames;
    }

    std::memcpy(
        music_frames + current_frames * 2, samples, sizeof(f32) * frames * 2);
    current_frames += frames;
}

const u32 N = 1 << 13; // up to just around 20kHz, ignore a bunch of high
                       // frequencies, anyways frequency is not linear
Complex in[N] = {0};
Complex out[N] = {0};

f32 amp(Complex a) {
    f32 c = a.real();
    f32 s = a.imag();
    f32 mag2 = c * c + s * s;
    return std::sqrt(mag2);
}
f32 max_amp = 0.1f;

void processor(void *buffer, u32 frames) {
    f32 *samples = (f32 *)buffer; // Samples internally stored as <float>s
    // wave(samples, frames);

    // load the input array
    for (u32 i = 0; i < frames; ++i) {
        memmove(in, in + 1, (N - 1) * sizeof(Complex));
        in[N - 1] = samples[i * 2];
    }
    // perform fft
    fft(in, out, 1, N);
    for (u32 i = 0; i < N; ++i) {
        f32 a = amp(out[i]);
        if (a > max_amp) {
            max_amp = a;
        }
    }
}

i32 main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(window_width, window_height, "Boum");
    InitAudioDevice();
    Music music = LoadMusicStream("./res/dungeon.ogg");
    // Music music = LoadMusicStream("./res/sample.mp3");
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, processor);
    printf("%i %i %i %i\n",
           music.stream.sampleSize,
           music.stream.sampleRate,
           music.stream.channels,
           music.frameCount);

    SetTargetFPS(60);

    RenderTexture2D fbo;
    Shader shader = LoadShader("./res/shader.rect.vs", "./res/shader/rect.fs");
    Matrix mat;
    mat = MatrixOrtho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "mvp"), mat);

    Texture2D default_tex = {
        rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    SetShapesTexture(default_tex, (Rectangle){0.0f, 0.0f, 1.0f, 1.0f});

    Video video(1920, 1080);
    video.ffmpeg_start("output.mp4", 30);

    while (!WindowShouldClose()) {
        f32 dt = GetFrameTime();
        UpdateMusicStream(music); // Update music buffer with new stream data
        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music)) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }

        BeginDrawing();
        ClearBackground(Color(10.0f, 10.0f, 10.0f, 255.0f));

        f32 w = GetScreenWidth(), h = GetScreenHeight();

        // BeginTextureMode(fbo);
        f32 cell_width = 16;
        f32 scale = 200.0f;
        for (u32 i = 1; i < N; ++i) {
            f32 value = amp(out[i]) / max_amp;
            if (i * cell_width > w) {
                break;
            }
            Color color = hsv_to_rgb(i * cell_width / w * 360.0f, 0.7f, 0.85f);
            BeginShaderMode(shader);
            DrawRectangle(i * cell_width,
                          h / 2.0f - value * scale,
                          cell_width - 1,
                          value * scale * 2.0f,
                          color);
            EndShaderMode();
        }
        // EndTextureMode();
        // video.write_frame((u32 *)LoadImageFromTexture(fbo.texture).data);
        // DrawTexture(fbo.texture, 0.0f, 0.0f, RAYWHITE);
        EndDrawing();
    }
    video.ffmpeg_end();

    UnloadShader(shader);

    DetachAudioStreamProcessor(music.stream, processor);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

i32 main2() {
    u32 n = 16;
    Complex nums[n];
    for (u32 i = 0; i < n; ++i) {
        f32 t = (f32)i / n;
        nums[i] = std::cos(2.0f * PI * t) + std::sin(2.0f * PI * t * 2) +
                  std::cos(2.0f * PI * t * 3);
    }
    Complex outd[n], outf[n];
    dft(nums, outd, n);
    fft(nums, outf, 1, n);
    for (u32 i = 0; i < n; ++i) {
        printf("%u: dft: %.2f %.2f fft: %.2f %.2f\n",
               i,
               outd[i].real(),
               outd[i].imag(),
               outf[i].real(),
               outf[i].imag());
    }
    return 0;
}

i32 main1() {
    Video video(1600, 900);
    video.ffmpeg_start("output.mp4", 30);
    u32 screen[1600 * 900];
    for (u32 y = 0; y < 900; ++y) {
        for (u32 x = 0; x < 1600; ++x) {
            if ((y + x) % 2 == 0) {
                screen[y * 1600 + x] = 0xffffffff;
            } else {
                screen[y * 1600 + x] = 0xff00ffff;
            }
        }
    }
    // 4 seconds
    for (u32 i = 0; i < 120; ++i) {
        video.write_frame(screen);
    }
    video.ffmpeg_end();
}
