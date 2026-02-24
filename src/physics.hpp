#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>
#include <sys/types.h>
#include <tuple>
#include <vector>

/**
 * A vector consisting of two doubles.
 */
class DVector2 {
public:
  /**
   * The constructor for the DVector2 class.
   */
  DVector2(double x, double y) : x(x), y(y) {}
  DVector2() : x(0.0), y(0.0) {}

  /**
   * Returns the x, or first, value in the vector
   */
  double get_x() const;

  /**
   * Returns the y, or second, value in the vector
   */
  double get_y() const;

  /**
   * Returns the magnitude of the vector
   */
  double magnitude() const;

  friend DVector2 operator+(DVector2 left, const DVector2 &right);

  friend DVector2 operator*(DVector2 left, double scalar);

  friend DVector2 operator*(double scalar, DVector2 right);

  friend DVector2 operator/(DVector2 left, double scalar);

  friend bool operator==(DVector2 left, const DVector2 &right);

private:
  double x;
  double y;
};

class RGB {
public:
  RGB(ushort red, ushort green, ushort blue);
  const ushort get_red() const;
  const ushort get_green() const;
  const ushort get_blue() const;

private:
  const std::tuple<ushort, ushort, ushort> color;
};

typedef struct {
  double mass;
  double radius;
  DVector2 pos;
  DVector2 vel;
  RGB color;
} PhysicalObject;

class PhysicsStep {
public:
  virtual std::vector<DVector2>
  compute(const std::vector<PhysicalObject> &objects) = 0;
};

class GravityStrategy : public PhysicsStep {};

class NoGravity : public GravityStrategy {
  std::vector<DVector2>
  compute(const std::vector<PhysicalObject> &objects) override;
};

class PhysicsHandler {
public:
  PhysicsHandler();
  PhysicsHandler(std::vector<PhysicalObject> objects);
  const std::vector<PhysicalObject> &get_objects() const;
  void step(double delta_t);
  void setGravityStrat(std::unique_ptr<GravityStrategy> strat);

private:
  std::vector<PhysicalObject> objects;
  std::unique_ptr<GravityStrategy> gravityStrat;
};

#endif // !PHYSICS_H
