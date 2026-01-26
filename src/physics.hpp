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

    friend DVector2 operator*(DVector2 left, int scalar);

    friend DVector2 operator*(int scalar, DVector2 right);

  private:
    double x;
    double y;
};

class SimObject {
  public:
    /**
     * The constructor for the SimObject class.
     * Requires a mass (double), radius (float), position (DVector2), and an
     * optional velocity (DVector2).
     */
    SimObject(double mass, float radius, DVector2 position,
              DVector2 velocity = DVector2());

    /**
     * Returns the unique ID of the object
     */
    uint64_t get_id() const;

    /**
     * Returns the object's mass
     */
    double get_mass() const;

    /**
     * Returns the object's radius
     */
    float get_radius() const;

    /**
     * Returns the object's position
     */
    DVector2 get_position() const;

    /**
     * Returns the object's velocity
     */
    DVector2 get_velocity() const;

    /**
     * Sets the position of the object to the given value
     */
    void set_position(DVector2 position);

    /**
     * Sets the velocity of the object to the given value
     */
    void set_velocity(DVector2 velocity);

    /**
     * Sets the radius of the object to the given value
     */
    void set_radius(float radius);

    /**
     * Sets the mass of the object to the given value
     */
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
    /**
     * Adds an object to the simulation with the given mass, radius, and
     * position. Returns the unique ID that was given to the new object.
     */
    uint64_t add(double mass, float radius, DVector2 position);

    /**
     * Removes the object with the given ID from the simulation.
     * Returns true if the object was found and removed.
     */
    bool remove(uint64_t object_id);

    /**
     * Runs a step of the simulation for an in-simulation time of delta_t
     * seconds.
     */
    void run(double delta_t);

    /**
     * Returns a const reference to the vector of objects.
     */
    const std::vector<SimObject> &get_objects() const;

    /**
     * Returns an optional reference to the object with the given ID.
     */
    std::optional<std::reference_wrapper<const SimObject>>
    get_object(uint64_t object_id) const;

    /**
     * Sets the position of a specified object to the given value.
     */
    bool set_position(uint64_t object_id, DVector2 position);

    /**
     * Sets the mass of a specified object to the given value.
     */
    bool set_mass(uint64_t object_id, double mass);

    /**
     * Sets the velocity of a specified object to the given value.
     */
    bool set_velocity(uint64_t object_id, DVector2 velocity);

    /**
     * Sets the radius of a specified object to the given value.
     */
    bool set_radius(uint64_t object_id, float radius);

  private:
    std::vector<SimObject> objects;

    std::optional<std::reference_wrapper<SimObject>>
    find_object(uint64_t object_id);
};

#endif // !PHYSICS_H
