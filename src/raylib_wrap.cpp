#include "raylib_wrap.hpp"
#include "physics.hpp"
#include "raylib.h"

void applyCamMove(Camera2D &cam) {
    Vector2 target_change = {0.0f, 0.0f};
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        target_change.y -= 10.0f;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        target_change.y += 10.0f;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        target_change.x += 10.0f;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        target_change.x -= 10.0f;
    }
    cam.target.x += target_change.x / cam.zoom;
    cam.target.y += target_change.y / cam.zoom;
}

void handleInput(Camera2D &cam) {
    applyCamMove(cam);
}

void RaylibHandler::run() {
    InitWindow(1920, 1080, "Gravity!");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    Camera2D cam = {0};
    cam.offset = {width / 2.0f, height / 2.0f};
    cam.zoom = zoom;

    float delta_t;
    const std::vector<PhysicalObject> &objects = physicsHandler->get_objects();
    while (!WindowShouldClose()) {
        delta_t = GetFrameTime();
        physicsHandler->step(delta_t * simSpeed);
        width = GetScreenWidth();
        height = GetScreenHeight();
        cam.offset = {width / 2.0f, height / 2.0f};

        handleInput(cam);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cam);

        Vector2 topLeft = GetScreenToWorld2D({0, 0}, cam);
        Vector2 botRight = GetScreenToWorld2D(
            {static_cast<float>(width), static_cast<float>(height)}, cam);

        Rectangle inView = {topLeft.x, topLeft.y, botRight.x - topLeft.x,
                            botRight.y - topLeft.y};

        for (PhysicalObject obj : objects) {
            Rectangle objBox = {
                static_cast<float>(obj.pos.get_x() - obj.radius),
                static_cast<float>(obj.pos.get_y() - obj.radius),
                static_cast<float>(obj.radius * 2),
                static_cast<float>(obj.radius * 2)};
            if (CheckCollisionRecs(inView, objBox)) {
                Color color = {(u_char)obj.color.get_red(),
                               (u_char)obj.color.get_green(),
                               (u_char)obj.color.get_blue(), 255};
                DrawCircle(obj.pos.get_x(), obj.pos.get_y(), obj.radius, color);
            }
        }

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}
