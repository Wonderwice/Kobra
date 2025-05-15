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
    void scene::add_hittable(std::shared_ptr<hittable> object)
    {
        hittable_list.push_back(object);
    }
    const std::vector<std::shared_ptr<hittable>> scene::get_hittables() const
    {
        return hittable_list;
    }
}
