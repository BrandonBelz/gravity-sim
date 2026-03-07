#include "physics.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <memory>
#include <tuple>
#include <vector>

const double GRAVITY_CONST = 6.6743E-11;

double DVector2::get_x() const { return this->x; }

double DVector2::get_y() const { return this->y; }

double DVector2::magnitude() const {
  double x = this->x, y = this->y;
  return std::sqrt(x * x + y * y);
}

DVector2 operator+(DVector2 left, const DVector2 &right) {
  return DVector2(left.x + right.x, left.y + right.y);
}

DVector2 &DVector2::operator+=(const DVector2 &other) {
  *this = *this + other;
  return *this;
}

DVector2 operator-(DVector2 left, const DVector2 &right) {
  return DVector2(left.x - right.x, left.y - right.y);
}

DVector2 &DVector2::operator-=(const DVector2 &other) {
  *this = *this - other;
  return *this;
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

std::vector<DVector2>
RealGravity::compute(const std::vector<PhysicalObject> &objects) {
  std::vector<DVector2> forces(objects.size(), DVector2());

  for (size_t i = 0; i < objects.size(); i++) {
    for (size_t j = i + 1; j < objects.size(); j++) {
      PhysicalObject ob1 = objects[i];
      PhysicalObject ob2 = objects[j];
      DVector2 r_vec = ob2.pos - ob1.pos;

      double dist = r_vec.magnitude();
      double force_mag = GRAVITY_CONST * ob1.mass * ob2.mass / (dist * dist);
      double x_comp = force_mag * r_vec.get_x() / dist;
      double y_comp = force_mag * r_vec.get_y() / dist;
      DVector2 force_vec(x_comp, y_comp);
      forces[i] += force_vec;
      forces[j] -= force_vec;
    }
  }
  return forces;
}

PhysicsHandler::PhysicsHandler(double stepSize)
    : maxStepSize(stepSize), gravityStrat(std::make_unique<NoGravity>()) {}

PhysicsHandler::PhysicsHandler(double stepSize,
                               std::vector<PhysicalObject> objects)
    : maxStepSize(stepSize), objects(objects),
      gravityStrat(std::make_unique<NoGravity>()) {}

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
  size_t numSteps = static_cast<size_t>(delta_t / maxStepSize);
  double stepSize = delta_t / numSteps;
  for (size_t i = 0; i < numSteps; i++) {
    std::vector<DVector2> forces = gravityStrat->compute(objects);
    assert(forces.size() == objects.size());
    applyForces(objects, forces, stepSize);
  }
}

void PhysicsHandler::setGravityStrat(std::unique_ptr<GravityStrategy> strat) {
  gravityStrat = std::move(strat);
}

void PhysicsHandler::setMaxStepSize(double stepSize) { maxStepSize = stepSize; }

double PhysicsHandler::getMaxStepSize() const { return maxStepSize; }
