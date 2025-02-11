#include <raylib.h>

#include <cmath>
#include <cstring>
#include <iostream>

#include "util/types.hpp"

static constexpr i32 window_width = 1600, window_height = 900;

static constexpr u32 num_frames = 1500;
f32 music_frames[num_frames * 2] = {0.0f};
u32 current_frames = 0;

void processor(void *buffer, u32 frames) {
    f32 *samples = (f32 *)buffer; // Samples internally stored as <float>s
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
    // if (frames <= num_frames - current_frames) {
    //     memcpy(music_frames + current_frames * 2,
    //            samples,
    //            sizeof(f32) * frames * 2);
    //     current_frames += frames;
    // } else if (frames <= num_frames) {
    //     memmove(music_frames,
    //             music_frames + frames * 2,
    //             sizeof(f32) * 2 * (num_frames - frames));
    //     memcpy(music_frames + (num_frames - frames) * 2,
    //            samples,
    //            sizeof(f32) * 2 * frames);
    // } else {
    //     memcpy(music_frames, samples, sizeof(f32) * 2 * num_frames);
    //     current_frames = num_frames;
    // }
}

i32 main() {
    InitWindow(window_width, window_height, "Boum");
    InitAudioDevice();
    // Music music = LoadMusicStream("./res/dungeon.ogg");
    Music music = LoadMusicStream("./res/sample.mp3");
    SetMusicVolume(music, 1.5f);
    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, processor);
    printf("%i %i %i %i\n",
           music.stream.sampleSize,
           music.stream.sampleRate,
           music.stream.channels,
           music.frameCount);

    SetTargetFPS(60);
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

        for (i32 i = 0; i < num_frames; i++) {
            f32 t = music_frames[i * 2];
            DrawRectangle(
                1 + i, window_height / 2 - (i32)(t * 32), 1, t * 64, GOLD);
        }

        EndDrawing();
    }

    DetachAudioStreamProcessor(music.stream, processor);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
