#pragma once
#include "core/vec3.h"
#include <memory>

namespace cobra
{
  /**
   * @class texture
   * @brief Abstract base class for textures in the ray tracing engine.
   *
   * A texture can be evaluated at a 3D point with associated UV coordinates.
   * Subclasses implement concrete behaviors such as constant color, checker patterns, images, etc.
   */
  class texture
  {
  public:
    virtual ~texture() = default;

    /**
     * @brief Returns the color value of the texture at a given point.
     *
     * @param u Horizontal texture coordinate.
     * @param v Vertical texture coordinate.
     * @param p The 3D point in space where the texture is being sampled.
     * @return The color at the given point.
     */
    virtual vec3 value(double u, double v, const vec3 &p) const = 0;
  };

  /**
   * @class solid_color
   * @brief Texture that always returns a single constant color.
   */
  class solid_color : public texture
  {
  public:
    /**
     * @brief Constructs a solid color texture from a color vector.
     * @param albedo The constant color value.
     */
    solid_color(const vec3 &albedo) : albedo(albedo) {}

    /**
     * @brief Constructs a solid color texture from RGB components.
     * @param red Red component.
     * @param green Green component.
     * @param blue Blue component.
     */
    solid_color(double red, double green, double blue)
        : solid_color(vec3(red, green, blue)) {}

    /**
     * @brief Returns the constant color regardless of input.
     */
    vec3 value(double u, double v, const vec3 &p) const override
    {
      return albedo;
    }

  private:
    vec3 albedo; ///< The constant color stored in the texture.
  };

  /**
   * @class checker_texture
   * @brief 3D checkerboard texture that alternates between two textures or colors.
   *
   * The checker pattern alternates based on the 3D spatial coordinates (not UV),
   * creating a pattern of cubes in space.
   */
  class checker_texture : public texture
  {
  public:
    /**
     * @brief Constructs a checker pattern from two textures and a scale.
     * @param scale Frequency of the checker pattern. Higher values = smaller tiles.
     * @param even Texture used for "even" cells.
     * @param odd Texture used for "odd" cells.
     */
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {}

    /**
     * @brief Constructs a checker pattern from two solid colors.
     * @param scale Frequency of the checker pattern.
     * @param c1 Color used for "even" cells.
     * @param c2 Color used for "odd" cells.
     */
    checker_texture(double scale, const vec3 &c1, const vec3 &c2)
        : checker_texture(scale,
                          std::make_shared<solid_color>(c1),
                          std::make_shared<solid_color>(c2)) {}

    /**
     * @brief Evaluates the checker texture at a given point.
     *
     * Alternates between even and odd textures based on the sum of integer coordinates.
     */
    vec3 value(double u, double v, const vec3 &p) const override
    {
      auto xInteger = int(std::floor(inv_scale * p.x()));
      auto yInteger = int(std::floor(inv_scale * p.y()));
      auto zInteger = int(std::floor(inv_scale * p.z()));

      bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

      return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;              ///< Inverse of the pattern scale (used for coordinate scaling).
    std::shared_ptr<texture> even; ///< Texture for even checker cells.
    std::shared_ptr<texture> odd;  ///< Texture for odd checker cells.
  };
} // namespace cobra
