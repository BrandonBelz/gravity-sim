#include "physics.hpp"
#include "raylib_wrap.hpp"
#include <memory>
#include <vector>

int main(void) {
  std::vector<PhysicalObject> objs;
  objs.push_back((PhysicalObject){50.0, 20.0, DVector2(3.0, 3.0),
                                  DVector2(20.0, -3.0), RGB(255, 255, 255)});
  objs.push_back((PhysicalObject){50.0, 20.0, DVector2(40.0, -3.0),
                                  DVector2(3.0, -20.0), RGB(0, 255, 0)});

  GraphicsHandler *window =
      new RaylibHandler(std::make_unique<PhysicsHandler>(objs));
  window->run();
  delete window;
  return 0;
}
