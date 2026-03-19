#include "raylib_wrap.hpp"
#include "raylib.h"

typedef struct {
    double speed;
    bool paused;
} SpeedState;

typedef struct {
    int width;
    int height;
    float frameTime;
} WindowState;

void updateWindowState(WindowState &window) {
    window.frameTime = GetFrameTime();
    window.width = GetScreenWidth();
    window.height = GetScreenHeight();
}

typedef struct {
    SpeedState speedState;
    Camera2D cam;
    int selectedObjectIndex;
    WindowState window;
    const std::vector<PhysicalObject> &objects;
} SimState;

Vector2 dVector2ToVector2(DVector2 vec) {
    Vector2 newVec = {static_cast<float>(vec.get_x()),
                      static_cast<float>(vec.get_y())};
    return newVec;
}

void applyCamTarget(SimState &simState) {
    Camera2D &cam = simState.cam;
    if (simState.selectedObjectIndex < 0) {
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
    } else {
        DVector2 targetPos = simState.objects[simState.selectedObjectIndex].pos;
        cam.target.x = targetPos.get_x();
        cam.target.y = targetPos.get_y();
    }
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
    if (IsKeyDown(KEY_LEFT_ALT)) {
        speedState.speed *= 1.1;
    }
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speedState.speed /= 1.1;
    }
}

int selectObject(const SimState &state) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        return -1;
    } else if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return state.selectedObjectIndex;
    }
    bool found = false;
    double smallestRadius;
    int smallestObjectIndex;
    for (size_t i = 0; i < state.objects.size(); i++) {
        PhysicalObject obj = state.objects[i];
        Vector2 clickPosition =
            GetScreenToWorld2D(GetMousePosition(), state.cam);
        if (CheckCollisionPointCircle(clickPosition, dVector2ToVector2(obj.pos),
                                      obj.radius) &&
            (!found || obj.radius < smallestRadius)) {
            found = true;
            smallestRadius = obj.radius;
            smallestObjectIndex = i;
        }
    }
    if (!found) {
        return -1;
    }
    return smallestObjectIndex;
}

void handleInput(SimState &state) {
    state.selectedObjectIndex = selectObject(state);
    applyCamTarget(state);
    applyCamZoom(state.cam);
    applySpeedChanges(state.speedState);
}

bool objectIsInView(const PhysicalObject &obj, const SimState &state) {
    Rectangle objBox = {static_cast<float>(obj.pos.get_x() - obj.radius),
                        static_cast<float>(obj.pos.get_y() - obj.radius),
                        static_cast<float>(obj.radius * 2),
                        static_cast<float>(obj.radius * 2)};
    Vector2 topLeft = GetScreenToWorld2D({0, 0}, state.cam);
    Vector2 botRight =
        GetScreenToWorld2D({static_cast<float>(state.window.width),
                            static_cast<float>(state.window.height)},
                           state.cam);

    Rectangle inView = {topLeft.x, topLeft.y, botRight.x - topLeft.x,
                        botRight.y - topLeft.y};

    return CheckCollisionRecs(inView, objBox);
}

Color rgbToColor(RGB rgb) {
    return {(u_char)rgb.get_red(), (u_char)rgb.get_green(),
            (u_char)rgb.get_blue(), 255};
}

void RaylibHandler::run() {
    SimState state = {{simSpeed, false},
                      (Camera2D){0},
                      -1,
                      (WindowState){0, 0, 0.0},
                      physicsHandler->get_objects()};
    state.cam.zoom = zoom;

    InitWindow(1920, 1080, "Gravity!");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        updateWindowState(state.window);
        state.cam.offset = {state.window.width / 2.0f,
                            state.window.height / 2.0f};
        if (!state.speedState.paused)
            physicsHandler->step(state.window.frameTime *
                                 state.speedState.speed);

        BeginDrawing();
        handleInput(state);

        ClearBackground(BLACK);
        BeginMode2D(state.cam);

        for (PhysicalObject obj : state.objects) {
            if (objectIsInView(obj, state)) {
                DrawCircle(obj.pos.get_x(), obj.pos.get_y(), obj.radius,
                           rgbToColor(obj.color));
            }
        }

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}
