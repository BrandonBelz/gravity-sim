#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

/**
 * A vector consisting of two doubles.
 */
class DVector2 {
public:
  /**
   * The constructor for the DVector2 class.
   */
  DVector2(double x = 0.0, double y = 0.0);

  double get_x() const;

  double get_y() const;

  double magnitude() const;

  friend DVector2 operator+(DVector2 left, const DVector2 &right);

  friend DVector2 operator*(DVector2 left, int scalar);

  friend DVector2 operator*(int scalar, DVector2 right);

private:
  double x;
  double y;
};

class SimObject {
public:
  SimObject(double mass, float radius, DVector2 position,
            DVector2 velocity = DVector2());

  uint64_t get_id() const;
  double get_mass() const;
  float get_radius() const;
  DVector2 get_position() const;
  DVector2 get_velocity() const;

  void set_position(DVector2 position);
  void set_velocity(DVector2 velocity);
  void set_radius(float radius);
  void set_mass(double mass);

private:
  inline static uint64_t next_id = 0;
  uint64_t object_id;
  double mass;
  float radius;
  DVector2 position;
  DVector2 velocity;
};

class Simulation {
public:
  uint64_t add(double mass, float radius, DVector2 position);
  bool remove(uint64_t object_id);
  void run(double delta_t);
  const std::vector<SimObject> &get_objects() const;

  std::optional<std::reference_wrapper<const SimObject>>
  get_object(uint64_t object_id) const;

  bool set_position(uint64_t object_id, DVector2 position);
  bool set_mass(uint64_t object_id, double mass);
  bool set_velocity(uint64_t object_id, DVector2 velocity);
  bool set_radius(uint64_t object_id, float radius);

private:
  std::vector<SimObject> objects;

  std::optional<std::reference_wrapper<SimObject>>
  find_object(uint64_t object_id);
};

#endif // !PHYSICS_H
