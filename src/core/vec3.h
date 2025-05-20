#pragma once
#include <iostream>
#include <cmath>

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
    vec3() : e{0, 0, 0} {}

    /// Constructor with component values.
    /// @param e0 X component
    /// @param e1 Y component
    /// @param e2 Z component
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    /// @return The x-component of the vector.
    double x() const { return e[0]; }

    /// @return The y-component of the vector.
    double y() const { return e[1]; }

    /// @return The z-component of the vector.
    double z() const { return e[2]; }

    /// Unary minus operator.
    /// @return The negated vector.
    vec3 operator-() const
    {
      return vec3(-e[0], -e[1], -e[2]);
    }

    /// Const access to components via index.
    /// @param i Index (0 for x, 1 for y, 2 for z)
    double operator[](int i) const
    {
      return e[i];
    }

    /// Mutable access to components via index.
    /// @param i Index (0 for x, 1 for y, 2 for z)
    double &operator[](int i)
    {
      return e[i];
    }

    /// Adds another vector to this one.
    /// @param v Vector to add.
    /// @return Reference to this vector after addition.
    vec3 &operator+=(const vec3 &v)
    {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    /// Multiplies this vector by a scalar.
    /// @param t Scalar multiplier.
    /// @return Reference to this vector after scaling.
    vec3 &operator*=(double t)
    {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }

    /// Divides this vector by a scalar.
    /// @param t Scalar divisor.
    /// @return Reference to this vector after division.
    vec3 &operator/=(double t)
    {
      return *this *= 1 / t;
    }

    /// @return The Euclidean length (magnitude) of the vector.
    double length() const
    {
      return std::sqrt(length_squared());
    }

    /// @return The squared length of the vector (more efficient when exact length not needed).
    double length_squared() const
    {
      return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    /**
     * @brief Generates a random vector with components in the range [0, 1).
     * @return A random vec3.
     */
    static vec3 random()
    {
      return vec3(random_double(), random_double(), random_double());
    }

    /**
     * @brief Generates a random vector with components in the range [min, max).
     * @param min Minimum component value.
     * @param max Maximum component value.
     * @return A random vec3.
     */
    static vec3 random(double min, double max)
    {
      return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    /**
     * @brief Checks whether the vector is close to zero in all dimensions.
     * @return True if all components are near zero.
     */
    bool near_zero() const
    {
      auto s = 1e-8;
      return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }
  };

  // Vector Utility Function Declarations

  /**
   * @brief Give a random point in a the unit disk.
   * @return The random vector with z = 0.
   */
  inline vec3 random_in_unit_disk()
  {
    while (true)
    {
      auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
      if (p.length_squared() < 1)
        return p;
    }
  }

  /// Outputs a vector to an output stream.
  inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
  {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
  }

  /// Adds two vectors.
  inline vec3 operator+(const vec3 &u, const vec3 &v)
  {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
  }

  /// Subtracts one vector from another.
  inline vec3 operator-(const vec3 &u, const vec3 &v)
  {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
  }

  /// Performs component-wise multiplication of two vectors.
  inline vec3 operator*(const vec3 &u, const vec3 &v)
  {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
  }

  /// Multiplies a vector by a scalar (scalar * vector).
  inline vec3 operator*(double t, const vec3 &v)
  {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
  }

  /// Multiplies a vector by a scalar (vector * scalar).
  inline vec3 operator*(const vec3 &v, double t)
  {
    return t * v;
  }

  /// Divides a vector by a scalar.
  inline vec3 operator/(const vec3 &v, double t)
  {
    return (1 / t) * v;
  }

  /// Computes the dot product of two vectors.
  inline double dot(const vec3 &u, const vec3 &v)
  {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
  }

  /// Computes the cross product of two vectors.
  inline vec3 cross(const vec3 &u, const vec3 &v)
  {
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
  }

  /**
   * @brief Reflects a vector around a given normal.
   * @param v The incoming vector.
   * @param n The normal vector.
   * @return The reflected vector.
   */
  inline vec3 reflect(const vec3 &v, const vec3 &n)
  {
    return v - 2 * dot(v, n) * n;
  }

  /**
   * @brief Refracts a vector around a given normal.
   * @param uv The incoming vector.
   * @param n The normal vector.
   * @param Refractive index etai divided by etat.
   * @return The refracted vector.
   */
  inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
  {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
  }

  /// Returns a normalized (unit length) version of the vector.
  inline vec3 unit_vector(const vec3 &v)
  {
    return v / v.length();
  }

  /// Generate a random vector in the unit sphere.
  inline vec3 random_unit_vector()
  {
    {
      while (true)
      {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
          return p / sqrt(lensq);
      }
    }
  }

  /// Generate a random vector on the right hemisphere.
  inline vec3 random_on_hemisphere(const vec3 &normal)
  {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
      return on_unit_sphere;
    else
      return -on_unit_sphere;
  }
}
