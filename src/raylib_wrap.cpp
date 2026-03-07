#include "raylib_wrap.hpp"
#include "physics.hpp"
#include "raylib.h"
#include <sys/types.h>
#include <vector>

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

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(cam);

    Vector2 topLeft = GetScreenToWorld2D({0, 0}, cam);
    Vector2 botRight = GetScreenToWorld2D(
        {static_cast<float>(width), static_cast<float>(height)}, cam);

    Rectangle inView = {topLeft.x, topLeft.y, botRight.x - topLeft.x,
                        botRight.y - topLeft.y};

    for (PhysicalObject obj : objects) {
      Rectangle objBox = {static_cast<float>(obj.pos.get_x() - obj.radius),
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
