#include "physics.hpp"
#include <memory>
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

class GraphicsHandler {
public:
  GraphicsHandler(std::unique_ptr<PhysicsHandler> physicsHandler)
      : physicsHandler(std::move(physicsHandler)), simSpeed(1.0) {}
  virtual void run() = 0;
  void setZoom(double zoom) { this->zoom = zoom; }
  void setSpeed(double speed) { this->simSpeed = speed; }
  virtual ~GraphicsHandler() {};

protected:
  std::unique_ptr<PhysicsHandler> physicsHandler;
  double zoom;
  double simSpeed;
};

#endif // !GRAPHICS_HPP
