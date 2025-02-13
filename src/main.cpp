#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <valarray>

#include "math.hpp"
#include "util/types.hpp"

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

const u32 N = 256;
Complex in[N];
Complex out[N] = {0};

void processor(void *buffer, u32 frames) {
    f32 *samples = (f32 *)buffer; // Samples internally stored as <float>s
    wave(samples, frames);

    // load the input array
    for (u32 i = 0; i < N; ++i) {
        in[i] = samples[i * 2];
    }
    // perform fft
    fft(in, out, 1, N);
}

i32 main() {
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

    SetTargetFPS(30);
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
        ClearBackground(RAYWHITE);

        // for (i32 i = 0; i < num_frames; i++) {
        //     f32 t = music_frames[i * 2];
        //     DrawRectangle(
        //         1 + i, window_height / 2 - (i32)(t * 32), 1, t * 64, GOLD);
        // }
        f32 cell_width = (f32)window_width / N;
        for (u32 i = 0; i < N; ++i) {
            f32 value = std::max(out[i].real(), out[i].imag());
            // f32 value = in[i].real();
            DrawRectangle(i * cell_width,
                          window_height / 2 - value * 32.0f,
                          cell_width,
                          value * 32.0f,
                          RED);
        }
        EndDrawing();
    }

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
