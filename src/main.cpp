#include <raylib.h>

#include <iostream>

#include "util/types.hpp"

static constexpr i32 window_width = 1600, window_height = 900;

void processor(void *buffer_data, u32 frames) {
    // i32 *data = buffer_data;
}

i32 main() {
    InitWindow(window_width, window_height, "Boum");
    InitAudioDevice();
    Music music = LoadMusicStream("./res/dungeon.ogg");
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
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music)) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("SOME TEXT!!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
