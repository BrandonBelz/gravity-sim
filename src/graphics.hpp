#include "physics.hpp"
#include <memory>
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

class GraphicsHandler {
public:
  GraphicsHandler(std::unique_ptr<PhysicsHandler> physicsHandler)
      : physicsHandler(std::move(physicsHandler)) {}
  virtual void run() = 0;
  virtual ~GraphicsHandler() {};

protected:
  std::unique_ptr<PhysicsHandler> physicsHandler;
};

#endif // !GRAPHICS_HPP
