#include "geometry/hittable.h"

namespace cobra
{
    hittable::hittable(const vec3& color) : _color(color)
    {
    }

    hittable::~hittable()
    {
    }

    const vec3& cobra::hittable::color() const
    {
        return _color;
    }
}