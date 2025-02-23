#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <cmath>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

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
    f32 *samples = (f32 *)buffer; // Samples internally stored as float
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

i32 main(i32 argc, char **argv) {
    std::string output = "";
    u32 fps = 30;
    // check if there are a valid number of command line arguments
    if (argc == 2) {
    } else if (argc == 3) {
        output = argv[2];
    } else if (argc == 4) {
        output = argv[2];
        fps = std::stoi(argv[3]);
    } else {
        std::cerr << "ERROR: Please open 1 file and optionally 1 output file, "
                     "and the fps!\n";
        std::cerr << "USAGE: ./boum <path-to-file> <output-file> <fps> \n";
        return -1;
    }
    // extract file path
    std::string music_file_path = argv[1];
    if (!std::filesystem::exists(music_file_path)) {
        std::cerr << "ERROR: Failed to find file: '" << music_file_path
                  << "'\n";
        return -2;
    }

    // configure raylib, open window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(window_width, window_height, "BOUM - Éxplosions pour la teuf!");
    InitAudioDevice();
    // load music, set volume
    Music music = LoadMusicStream(music_file_path.c_str());
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    AttachAudioStreamProcessor(music.stream, processor);

    RenderTexture2D fbo = LoadRenderTexture(window_width, window_height);

    // shader uniforms
    Shader shader = LoadShader("./res/shader.rect.vs", "./res/shader/rect.fs");
    Matrix mat;
    mat = MatrixOrtho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "mvp"), mat);

    // set default texture for shape renderer, so we can use shaders
    Texture2D default_tex = {
        rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    SetShapesTexture(default_tex, (Rectangle){0.0f, 0.0f, 1.0f, 1.0f});

    Video video(1600, 900);
    if (!output.empty()) {
        video.ffmpeg_start(music_file_path, "output.mp4", fps);
    }
    SetTargetFPS(fps);
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

        f32 w = GetScreenWidth(), h = GetScreenHeight();

        // draw to texture
        BeginTextureMode(fbo);
        ClearBackground(Color(10.0f, 10.0f, 10.0f, 255.0f));
        f32 cell_width = 16.0f;
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
        EndTextureMode();
        // write data to file
        if (!output.empty()) {
            video.write_frame((u32 *)LoadImageFromTexture(fbo.texture).data);
        }

        // draw the visual onscreen
        BeginDrawing();
        ClearBackground(Color(10.0f, 10.0f, 10.0f, 255.0f));
        DrawTexture(fbo.texture, 0.0f, 0.0f, RAYWHITE);
        EndDrawing();
    }
    if (!output.empty()) {
        video.ffmpeg_end();
    }

    // detach, free all resources
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
