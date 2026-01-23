#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include <vector>

class DVector2 {
public:
  DVector2(double x, double y);
  double get_x() const;
  double get_y() const;

private:
  double x;
  double y;
};

class SimObject {
public:
  SimObject(double mass, float radius, DVector2 position,
            DVector2 velocity = {0.0, 0.0});

  double get_mass() const;
  float get_radius() const;
  DVector2 get_position() const;
  DVector2 get_velocity() const;

  void set_position(DVector2 position);
  void set_velocity(DVector2 velocity);

private:
  double mass;
  float radius;
  DVector2 position;
  DVector2 velocity;
};

class Simulation {
public:
  Simulation();
  uint64_t add(double mass, float radius, DVector2 position);
  void remove(uint64_t object_id);
  void run(double delta_t);
  const std::vector<SimObject> &get_objects() const;

  void set_position(uint64_t object_id, DVector2 position);
  void set_mass(uint64_t object_id, double mass);
  void set_velocity(uint64_t object_id, DVector2 velocity);
  void set_radius(uint64_t object_id, float radius);

private:
  std::vector<SimObject> objects;
  uint64_t next_id;
};

#endif // !PHYSICS_H
