#ifndef PHYSICS_H
#define PHYSICS_H

#include <utility>
#include <vector>

class DVector2 {
  double x;
  double y;

public:
  DVector2(double x, double y);
  double get_x() const;
  double get_y() const;
};

class Simulation {
  class SimObject {
    double mass;
    float radius;
    DVector2 position;
    DVector2 velocity;

  public:
    SimObject(double mass, float radius, DVector2 position, DVector2 velocity);
    SimObject(double mass, float radius, DVector2 position);

    double get_mass() const;
    float get_radius() const;
    DVector2 get_position() const;
    DVector2 get_velocity() const;

    void set_position(DVector2 position);
    void set_velocity(DVector2 velocity);
  };
  std::vector<SimObject> objects;

public:
  Simulation();
  int add(double mass, float radius, DVector2 position);
  std::pair<int, int> remove(int object_id);
  void run(double delta_t);

  DVector2 get_position(int object_id) const;
  double get_mass(int object_id) const;
  DVector2 get_velocity(int object_id) const;
  float get_radius(int object_id) const;
  const std::vector<DVector2> &get_positions() const;

  void set_position(int object_id, DVector2 position);
  void set_mass(int object_id, double mass);
  void set_velocity(int object_id, DVector2 velocity);
  void set_radius(int object_id, float radius);
};

#endif // !PHYSICS_H
