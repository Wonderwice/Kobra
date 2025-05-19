#include "core/ray.h"
#include "ray.h"

namespace cobra
{
    ray::ray()
    {
    }

    ray::ray(const vec3 &origin, const vec3 &direction) : origin(origin), direction(cobra::unit_vector(direction))
    {
    }

    ray::~ray()
    {
    }

    const vec3 &ray::get_origin() const
    {
        return origin;
    }

    const vec3 &ray::get_direction() const
    {
        return direction;
    }

    vec3 ray::at(double t) const
    {
        return origin + t * direction;
    }
    ray &ray::operator=(const cobra::ray &other)
    {
        if (this != &other)
        {
            origin = other.origin;
            direction = other.direction;
        }
        return *this;
    }

}
