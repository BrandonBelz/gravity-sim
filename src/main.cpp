#include "physics.hpp"
#include "raylib_wrap.hpp"
#include <memory>
#include <vector>

int main(void) {
    std::vector<PhysicalObject> objs;
    objs.push_back((PhysicalObject){5.972E24, 6371000.0, DVector2(0.0, 0.0),
                                    DVector2(0.0, -0.0125), RGB(0, 0, 255)});
    objs.push_back((PhysicalObject){7.348E22, 1737000.0, DVector2(384400000.0, 0.0),
                                    DVector2(0.0, 1018.0), RGB(130, 130, 130)});

    auto physicsHandler = std::make_unique<PhysicsHandler>(1000.0, objs);
    physicsHandler->setGravityStrat(std::make_unique<RealGravity>());
    GraphicsHandler *window = new RaylibHandler(std::move(physicsHandler));
    window->setZoom(0.000002);
    window->setSpeed(80000.0);
    window->run();
    delete window;
    return 0;
}
