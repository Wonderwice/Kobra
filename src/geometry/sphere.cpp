#include "geometry/sphere.h"
#include "geometry/hittable.h"
#include "core/vec3.h"
#include <cmath>
#include "hittable.h"

cobra::sphere::sphere(const vec3 &center, double radius, std::shared_ptr<material> mat) : _mat(mat), _center(center), _radius(radius)
{
    auto rvec = vec3(radius,radius,radius);
    bbox = aabb(center -rvec,center + rvec);
}

cobra::sphere::~sphere()
{
}

bool cobra::sphere::hit(const ray &r, interval ray_t, hit_record &rec) const
{
    vec3 oc = r.get_origin() - _center;
    auto a = dot(r.get_direction(), r.get_direction());
    auto half_b = dot(oc, r.get_direction());
    auto c = dot(oc, oc) - _radius * _radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;

    auto sqrt_d = std::sqrt(discriminant);
    double root = (-half_b - sqrt_d) / a;

    if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrt_d) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    vec3 outward_normal = (rec.point - _center) / _radius;
    rec.set_face_normal(r,outward_normal);
    rec.mat = mat();

    return true;
}

