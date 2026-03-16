#include "raylib_wrap.hpp"
#include "physics.hpp"
#include "raylib.h"
#include <vector>

typedef struct {
  double speed;
  bool paused;
} SpeedState;

typedef struct {
  SpeedState speedState;
  Camera2D cam;
  float frameTime;
  int screenWidth;
  int screenHeight;
  const std::vector<PhysicalObject> &objects;
} SimState;

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

void applyCamZoom(Camera2D &cam) {
  int mouseMove = GetMouseWheelMove();
  if (mouseMove > 0) {
    for (int i = 0; i < mouseMove; i++) {
      cam.zoom *= 1.3;
    }
  } else if (mouseMove < 0) {
    for (int i = 0; i > mouseMove; i--) {
      cam.zoom /= 1.3;
    }
  }
}

void applySpeedChanges(SpeedState &speedState) {
  if (IsKeyPressed(KEY_SPACE)) {
    speedState.paused = !speedState.paused;
  }
}

void handleInput(SimState &state) {
  applyCamMove(state.cam);
  applyCamZoom(state.cam);
  applySpeedChanges(state.speedState);
}

void RaylibHandler::run() {
  SimState state = {
      {simSpeed, false}, (Camera2D){0},     0.0f,
      GetScreenWidth(),  GetScreenHeight(), physicsHandler->get_objects()};

  InitWindow(1920, 1080, "Gravity!");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  state.cam.offset = {state.screenWidth / 2.0f, state.screenHeight / 2.0f};
  state.cam.zoom = zoom;

  while (!WindowShouldClose()) {
    state.frameTime = GetFrameTime();
    state.screenWidth = GetScreenWidth();
    state.screenHeight = GetScreenHeight();
    state.cam.offset = {state.screenWidth / 2.0f, state.screenHeight / 2.0f};

    BeginDrawing();
    handleInput(state);

    if (!state.speedState.paused)
      physicsHandler->step(state.frameTime * state.speedState.speed);
    ClearBackground(BLACK);
    BeginMode2D(state.cam);

    Vector2 topLeft = GetScreenToWorld2D({0, 0}, state.cam);
    Vector2 botRight =
        GetScreenToWorld2D({static_cast<float>(state.screenWidth),
                            static_cast<float>(state.screenHeight)},
                           state.cam);

    Rectangle inView = {topLeft.x, topLeft.y, botRight.x - topLeft.x,
                        botRight.y - topLeft.y};

    for (PhysicalObject obj : state.objects) {
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
