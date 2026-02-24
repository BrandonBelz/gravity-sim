#include "raylib_wrap.hpp"
#include "physics.hpp"
#include "raylib.h"
#include <sys/types.h>
#include <vector>

void RaylibHandler::run() {
  InitWindow(800, 600, "Gravity!");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  int width = GetScreenWidth();
  int height = GetScreenHeight();

  Camera2D cam = {0};
  cam.offset = {width / 2.0f, height / 2.0f};
  cam.zoom = 1.0f;

  float delta_t;
  const std::vector<PhysicalObject> &objects = physicsHandler->get_objects();
  while (!WindowShouldClose()) {
    delta_t = GetFrameTime();
    physicsHandler->step(delta_t);
    width = GetScreenWidth();
    height = GetScreenHeight();
    cam.offset = {width / 2.0f, height / 2.0f};

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(cam);

    for (PhysicalObject obj : objects) {
      Color color = {(u_char)obj.color.get_red(), (u_char)obj.color.get_green(),
                     (u_char)obj.color.get_blue(), 255};
      DrawCircle(obj.pos.get_x(), obj.pos.get_y(), obj.radius, color);
    }

    EndMode2D();
    EndDrawing();
  }
  CloseWindow();
}
