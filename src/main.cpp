#include <raylib.h>

#include "util/types.hpp"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

static constexpr i32 window_width = 1600, window_height = 900;

struct Global {
    Texture texture;
} g;

void init() {
    g.texture = LoadTexture("./res/scarfy.png");
}

void input(f32 dt) {}

void update(f32 dt) {}

void render(f32 dt) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("SOME TEXT!!", 190, 200, 20, LIGHTGRAY);

    DrawTexture(g.texture, 0, 0, WHITE);
    EndDrawing();
}

void quit() {
    UnloadTexture(g.texture);
    CloseWindow();
}

void frame();

int main() {
    InitWindow(window_width, window_height, "GAME");
    init();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(frame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        frame();
    }
#endif

    quit();

    return 0;
}

void frame() {
    f32 dt = GetFrameTime();
    input(dt);
    update(dt);
    render(dt);
}
