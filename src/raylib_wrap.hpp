#include "graphics.hpp"
#ifndef RAYLIB_WRAP
#define RAYLIB_WRAP

class RaylibHandler : public GraphicsHandler {
public:
    using GraphicsHandler::GraphicsHandler;
    void run() override;
};

#endif // !RAYLIB_WRAP
