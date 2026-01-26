#include "physics.hpp"
#include <cmath>

DVector2::DVector2(double x, double y) { this->x = x, this->y = y; }

double DVector2::get_x() const { return this->x; }

double DVector2::get_y() const { return this->y; }

double DVector2::magnitude() const {
  double x = this->get_x(), y = this->get_y();
  return std::sqrt(x * x + y * y);
}

DVector2 operator+(DVector2 left, const DVector2 &right) {
  return DVector2(left.x + right.x, left.y + right.y);
}

DVector2 operator*(DVector2 left, int scalar) {
  return DVector2(left.x * scalar, left.y * scalar);
}

DVector2 operator*(int scalar, DVector2 right) { return right * scalar; }

SimObject::SimObject(double mass, float radius, DVector2 position,
                     DVector2 velocity) {
  this->object_id = next_id, this->mass = mass, this->radius = radius;
  this->position = position, this->velocity = velocity;
  next_id++;
}

uint64_t SimObject::get_id() const { return this->object_id; }

double SimObject::get_mass() const { return this->mass; }

float SimObject::get_radius() const { return this->radius; }

DVector2 SimObject::get_position() const { return this->position; }

DVector2 SimObject::get_velocity() const { return this->velocity; }

void SimObject::set_position(DVector2 position) { this->position = position; }

void SimObject::set_velocity(DVector2 velocity) { this->velocity = velocity; }

void SimObject::set_radius(float radius) { this->radius = radius; }

void SimObject::set_mass(double mass) { this->mass = mass; }

uint64_t Simulation::add(double mass, float radius, DVector2 position) {
  auto new_obj = SimObject(mass, radius, position);
  this->objects.push_back(new_obj);
  return new_obj.get_id();
}

bool Simulation::remove(uint64_t object_id) {
  auto vec_size = this->objects.size();
  bool found = false;
  for (size_t i = 0; i < vec_size; i++) {
    if (this->objects[i].get_id() == object_id) {
      this->objects[i] = this->objects.back();
      this->objects.pop_back();
      found = true;
      break;
    }
  }
  return found;
}

void Simulation::run(double delta_t) {
  for (auto object : this->objects) {
    DVector2 delta_x = object.get_velocity() * delta_t;
    object.set_position(object.get_position() + delta_x);
  }
}

const std::vector<SimObject> &Simulation::get_objects() const {
  return this->objects;
}

std::optional<std::reference_wrapper<const SimObject>>
Simulation::get_object(uint64_t object_id) const {
  for (int i = 0; i < this->objects.size(); i++) {
    if (this->objects[i].get_id() == object_id) {
      return this->objects[i];
    }
  }
  return std::nullopt;
}

std::optional<std::reference_wrapper<SimObject>>
Simulation::find_object(uint64_t object_id) {
  for (int i = 0; i < this->objects.size(); i++) {
    if (this->objects[i].get_id() == object_id) {
      return this->objects[i];
    }
  }
  return std::nullopt;
}

bool Simulation::set_position(uint64_t object_id, DVector2 position) {
  if (auto object = this->find_object(object_id)) {
    object->get().set_position(position);
    return true;
  }
  return false;
}

bool Simulation::set_mass(uint64_t object_id, double mass) {
  if (auto object = this->find_object(object_id)) {
    object->get().set_mass(mass);
    return true;
  }
  return false;
}

bool Simulation::set_velocity(uint64_t object_id, DVector2 velocity) {
  if (auto object = this->find_object(object_id)) {
    object->get().set_velocity(velocity);
    return true;
  }
  return false;
}

bool Simulation::set_radius(uint64_t object_id, float radius) {
  if (auto object = this->find_object(object_id)) {
    object->get().set_radius(radius);
    return true;
  }
  return false;
}
