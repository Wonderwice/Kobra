#include "geometry/sphere.h"
#include "geometry/hittable.h"
#include "core/vec3.h"
#include <cmath>
#include "hittable.h"
#include "core/onb.h"

cobra::sphere::sphere(const vec3 &center, double radius, std::shared_ptr<material> mat) : _mat(mat), _center(center), _radius(radius)
{
    auto rvec = vec3(radius, radius, radius);
    bbox = aabb(center - rvec, center + rvec);
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

    if (discriminant < 0)
        return false;

    auto sqrt_d = std::sqrt(discriminant);
    double root = (-half_b - sqrt_d) / a;

    if (!ray_t.surrounds(root))
    {
        root = (-half_b + sqrt_d) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    vec3 outward_normal = (rec.point - _center) / _radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat();

    return true;
}

double cobra::sphere::pdf_value(const vec3 &origin, const vec3 &direction) const
{
    // This method only works for stationary spheres.

    hit_record rec;
    if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
        return 0;

    auto dist_squared = (_center - origin).length_squared();
    auto cos_theta_max = std::sqrt(1 - _radius * _radius / dist_squared);
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

cobra::vec3 cobra::sphere::random(const vec3 &origin) const
{
    vec3 direction = _center - origin;
    auto distance_squared = direction.length_squared();
    onb uvw(direction);
    return uvw.transform(random_to_sphere(_radius, distance_squared));
}

cobra::vec3 cobra::sphere::random_to_sphere(double radius, double distance_squared)
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * pi * r1;
    auto x = std::cos(phi) * std::sqrt(1 - z * z);
    auto y = std::sin(phi) * std::sqrt(1 - z * z);

    return vec3(x, y, z);
}