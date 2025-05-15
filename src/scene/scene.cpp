#include "scene/scene.h"
#include "scene.h"

namespace cobra
{
    scene::scene() : hittable_list()
    {
    }

    scene::~scene()
    {
    }
    void scene::add_hittable(hittable* object)
    {
        hittable_list.push_back(object);
    }
    const std::vector<hittable*> scene::get_hittables() const
    {
        return hittable_list;
    }
}
