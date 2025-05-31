#pragma once
#include "geometry/hittable.h"

namespace cobra
{
    /**
     * @class quad
     * @brief Represents a planar quadrilateral surface for ray tracing.
     *
     * The `quad` class defines a parallelogram in 3D space using an origin point `Q`
     * and two edge vectors `u` and `v`. It computes the plane, bounding box, and handles
     * ray intersection logic.
     */
    class quad : public hittable
    {
    private:
        vec3 Q;                   ///< Origin point of the quad
        vec3 u, v;                ///< Edge vectors defining the parallelogram
        vec3 w;                   ///< Helper vector for coordinate projection
        vec3 normal;              ///< Normal vector of the quad's plane
        double D;                 ///< Dot product of normal and point Q (for plane equation)
        shared_ptr<material> mat; ///< Material assigned to the quad
        aabb bbox;                ///< Axis-aligned bounding box for the quad
        double area;              ///< Area of the quad, useful for pdf calculations

    public:
        /**
         * @brief Constructs a quad given an origin, two edge vectors, and a material.
         *
         * The quad lies on the plane formed by `Q`, `Q + u`, and `Q + v`.
         *
         * @param Q The origin point of the quad.
         * @param u First edge vector.
         * @param v Second edge vector.
         * @param mat Shared pointer to the material used by the quad.
         */
        quad(const vec3 &Q, const vec3 &u, const vec3 &v, std::shared_ptr<material> mat)
            : Q(Q), u(u), v(v), mat(mat)
        {
            // Build a bounding box that encloses the quad
            aabb bbox_diagonal1 = aabb(Q, Q + u + v);
            aabb bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);

            // Compute the normal and plane equation
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n, n); // Used to compute (alpha, beta) barycentric-like coords
            area = n.length();
        }

        /**
         * @brief Default virtual destructor.
         */
        ~quad() = default;

        /**
         * @brief Returns the bounding box enclosing the quad.
         *
         * @return An AABB that tightly fits the quad.
         */
        aabb bounding_box() const override { return bbox; }

        /**
         * @brief Checks for ray-quad intersection.
         *
         * If the ray hits the quad's plane and the intersection point lies inside
         * the quadrilateral, fills the hit record and returns true.
         *
         * @param r The ray to test.
         * @param ray_t The valid range of t values for the ray.
         * @param rec Output parameter containing hit data if an intersection occurs.
         * @return true if the ray hits the quad, false otherwise.
         */
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            double denom = dot(normal, r.get_direction());

            // No hit if ray is parallel to the quad's plane
            if (std::fabs(denom) < 1e-8)
                return false;

            double t = (D - dot(normal, r.get_origin())) / denom;
            if (!ray_t.contains(t))
                return false;

            vec3 intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;

            // Compute "barycentric"-like coordinates to test if hit is inside the quad
            double alpha = dot(w, cross(planar_hitpt_vector, v));
            double beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.point = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        /**
         * @brief Determines if a point with local coordinates (alpha, beta) is inside the quad.
         *
         * The quad is defined in the unit square [0,1]x[0,1] in (alpha, beta) coordinates.
         *
         * @param a The alpha coordinate.
         * @param b The beta coordinate.
         * @param rec The hit record (can be modified or used for other purposes).
         * @return true if the point is within the quad, false otherwise.
         */
        virtual bool is_interior(double a, double b, hit_record &rec) const
        {
            interval unit_interval = interval(0, 1);
            return unit_interval.contains(a) && unit_interval.contains(b);
        }
        
        double pdf_value(const vec3 &origin, const vec3 &direction) const override
        {
            hit_record rec;
            if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
                return 0;

            auto distance_squared = rec.t * rec.t * direction.length_squared();
            auto cosine = std::fabs(dot(direction, rec.normal) / direction.length());

            return distance_squared / (cosine * area);
        }

        vec3 random(const vec3 &origin) const override
        {
            auto p = Q + (random_double() * u) + (random_double() * v);
            return p - origin;
        }
    };

    class cube : public scene
    {
    public:
        cube(const vec3 &a, const vec3 &b, shared_ptr<material> mat)
        {
            auto min = vec3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
            auto max = vec3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

            auto dx = vec3(max.x() - min.x(), 0, 0);
            auto dy = vec3(0, max.y() - min.y(), 0);
            auto dz = vec3(0, 0, max.z() - min.z());

            add_hittable(make_shared<quad>(vec3(min.x(), min.y(), max.z()), dx, dy, mat));  // front
            add_hittable(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
            add_hittable(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
            add_hittable(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dz, dy, mat));  // left
            add_hittable(make_shared<quad>(vec3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
            add_hittable(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dx, dz, mat));  // bottom
        }
    };
} // namespace cobra
