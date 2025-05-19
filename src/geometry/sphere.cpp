#include "geometry/sphere.h"
#include "geometry/hittable.h"
#include "core/vec3.h"
#include <cmath>
#include "hittable.h"

cobra::sphere::sphere(const vec3 &center, double radius, std::shared_ptr<material> mat) : hittable(mat), _center(center), _radius(radius)
{
}

cobra::sphere::~sphere()
{
}

bool cobra::sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.get_origin() - _center;
    auto a = dot(r.get_direction(), r.get_direction());
    auto half_b = dot(oc, r.get_direction());
    auto c = dot(oc, oc) - _radius * _radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;

    auto sqrt_d = std::sqrt(discriminant);
    double root = (-half_b - sqrt_d) / a;

    if (root < t_min || root > t_max) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    vec3 outward_normal = (rec.point - _center) / _radius;
    rec.set_face_normal(r,outward_normal);
    rec.mat = mat();

    return true;
}

