#include "physics.hpp"
#include <cassert>
#include <cmath>
#include <memory>
#include <tuple>

double DVector2::get_x() const { return this->x; }

double DVector2::get_y() const { return this->y; }

double DVector2::magnitude() const {
  double x = this->x, y = this->y;
  return std::sqrt(x * x + y * y);
}

DVector2 operator+(DVector2 left, const DVector2 &right) {
  return DVector2(left.x + right.x, left.y + right.y);
}

DVector2 operator*(DVector2 left, double scalar) {
  return DVector2(left.x * scalar, left.y * scalar);
}

DVector2 operator*(double scalar, DVector2 right) { return right * scalar; }

DVector2 operator/(DVector2 left, double scalar) {
  return DVector2(left.x / scalar, left.y / scalar);
}

bool operator==(DVector2 left, const DVector2 &right) {
  return left.x == right.x && left.y == right.y;
}

std::tuple<ushort, ushort, ushort> validateRGB(ushort red, ushort green,
                                               ushort blue) {
  if (red > 255) {
    red = 255;
  }
  if (green > 255) {
    green = 255;
  }
  if (blue > 255) {
    blue = 255;
  }
  return std::make_tuple(red, green, blue);
}

RGB::RGB(ushort red, ushort green, ushort blue)
    : color(validateRGB(red, green, blue)) {}

const ushort RGB::get_red() const { return std::get<0>(color); }

const ushort RGB::get_green() const { return std::get<1>(color); }

const ushort RGB::get_blue() const { return std::get<2>(color); }

std::vector<DVector2>
NoGravity::compute(const std::vector<PhysicalObject> &objects) {
  return std::vector<DVector2>(objects.size(), DVector2());
}

PhysicsHandler::PhysicsHandler()
    : gravityStrat(std::make_unique<NoGravity>()) {}

PhysicsHandler::PhysicsHandler(std::vector<PhysicalObject> objects)
    : objects(objects), gravityStrat(std::make_unique<NoGravity>()) {}

const std::vector<PhysicalObject> &PhysicsHandler::get_objects() const {
  return objects;
}

void applyForces(std::vector<PhysicalObject> &objects,
                 std::vector<DVector2> &forces, double t) {
  for (size_t i = 0; i < objects.size(); i++) {
    PhysicalObject &obj = objects[i];
    DVector2 accel = forces[i] / obj.mass;
    obj.pos = 0.5 * accel * t * t + obj.vel * t + obj.pos;
    obj.vel = accel * t + obj.vel;
  }
}

void PhysicsHandler::step(double delta_t) {
  std::vector<DVector2> forces = gravityStrat->compute(objects);

  assert(forces.size() == objects.size());
  applyForces(objects, forces, delta_t);
}

void PhysicsHandler::setGravityStrat(std::unique_ptr<GravityStrategy> strat) {
  gravityStrat = std::move(strat);
}
