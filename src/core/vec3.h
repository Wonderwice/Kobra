#pragma once
#include <iostream>

namespace cobra
{
  /**
   * @class vec3
   * @brief A class representing a 3D vector.
   *
   * This class provides basic vector operations such as addition, subtraction,
   * scalar multiplication, dot product, cross product, and normalization.
   */
  class vec3
  {
  public:
    double e[3]; ///< The vector components (x, y, z)

    /// Default constructor. Initializes the vector to (0, 0, 0).
    vec3();

    /// Constructor with component values.
    /// @param e0 X component
    /// @param e1 Y component
    /// @param e2 Z component
    vec3(double e0, double e1, double e2);

    /// @return The x-component of the vector.
    double x() const;

    /// @return The y-component of the vector.
    double y() const;

    /// @return The z-component of the vector.
    double z() const;

    /// Unary minus operator.
    /// @return The negated vector.
    vec3 operator-() const;

    /// Const access to components via index.
    /// @param i Index (0 for x, 1 for y, 2 for z)
    double operator[](int i) const;

    /// Mutable access to components via index.
    /// @param i Index (0 for x, 1 for y, 2 for z)
    double &operator[](int i);

    /// Adds another vector to this one.
    /// @param v Vector to add.
    /// @return Reference to this vector after addition.
    vec3 &operator+=(const vec3 &v);

    /// Multiplies this vector by a scalar.
    /// @param t Scalar multiplier.
    /// @return Reference to this vector after scaling.
    vec3 &operator*=(double t);

    /// Divides this vector by a scalar.
    /// @param t Scalar divisor.
    /// @return Reference to this vector after division.
    vec3 &operator/=(double t);

    /// @return The Euclidean length (magnitude) of the vector.
    double length() const;

    /// @return The squared length of the vector (more efficient when exact length not needed).
    double length_squared() const;

     /**
     * @brief Generates a random vector with components in the range [0, 1).
     * @return A random vec3.
     */
    static vec3 random();

    /**
     * @brief Generates a random vector with components in the range [min, max).
     * @param min Minimum component value.
     * @param max Maximum component value.
     * @return A random vec3.
     */
    static vec3 random(double min, double max);

    /**
     * @brief Checks whether the vector is close to zero in all dimensions.
     * @return True if all components are near zero.
     */
    bool near_zero() const;

    /**
     * @brief Reflects a vector around a given normal.
     * @param v The incoming vector.
     * @param n The normal vector.
     * @return The reflected vector.
     */
    static vec3 reflect(const vec3& v, const vec3& n);
  };

  /// Alias for vec3 used when representing 3D points.
  using point3 = vec3;

  // Vector Utility Function Declarations

  /// Outputs a vector to an output stream.
  std::ostream &operator<<(std::ostream &out, const vec3 &v);

  /// Adds two vectors.
  vec3 operator+(const vec3 &u, const vec3 &v);

  /// Subtracts one vector from another.
  vec3 operator-(const vec3 &u, const vec3 &v);

  /// Performs component-wise multiplication of two vectors.
  vec3 operator*(const vec3 &u, const vec3 &v);

  /// Multiplies a vector by a scalar (scalar * vector).
  vec3 operator*(double t, const vec3 &v);

  /// Multiplies a vector by a scalar (vector * scalar).
  vec3 operator*(const vec3 &v, double t);

  /// Divides a vector by a scalar.
  vec3 operator/(const vec3 &v, double t);

  /// Computes the dot product of two vectors.
  double dot(const vec3 &u, const vec3 &v);

  /// Computes the cross product of two vectors.
  vec3 cross(const vec3 &u, const vec3 &v);

  /// Returns a normalized (unit length) version of the vector.
  vec3 unit_vector(const vec3 &v);

  /// Generate a random vector in the unit sphere.
  vec3 random_unit_vector();

  /// Generate a random vector on the right hemisphere.
  vec3 random_on_hemisphere(const vec3 &normal);
}
