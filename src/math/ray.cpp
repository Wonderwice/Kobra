#include "math/ray.h"

namespace cobra
{
    ray::ray(const vec3 origin, const vec3 direction) : origin(origin), direction(direction)
    {
    }

    ray::~ray()
    {
    }
}