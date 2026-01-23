#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

class DVector2 {
  double x;
  double y;

public:
  DVector2(double x, double y);
  ~DVector2();
};

class SimObject {
  double mass;
  float radius;
  DVector2 position;
  DVector2 velocity;

public:
  SimObject();
  ~SimObject();
};

class Simulation {
  std::vector<SimObject> objects;

public:
  Simulation();
  ~Simulation();
  void add(double mass, float radius, DVector2 position);
  int remove(int object_id);

  DVector2 get_position(int object_id);
  double get_mass(int object_id);
  DVector2 get_velocity(int object_id);
  float get_radius(int object_id);
  std::vector<DVector2> get_positions();

  void set_position(int object_id, DVector2 position);
  void set_mass(int object_id, double mass);
  void set_velocity(int object_id, DVector2 velocity);
  void set_radius(int object_id, float radius);
};

#endif // !PHYSICS_H
