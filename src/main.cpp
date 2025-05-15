#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "renderer/raytracer.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"
#include "geometry/sphere.h"
#include <memory>

using namespace cobra;

int main(){
    const size_t image_width = 1280;
    const double aspect_ratio = 16./9.;

    std::shared_ptr<sphere> _sphere = std::make_shared<sphere>(vec3(0, 0, -1), 0.5, vec3(1, 0, 0));
    
    scene _scene = scene();
    _scene.add_hittable(_sphere);

    raytracer ray_tracer(10, 1, camera(image_width,aspect_ratio,2,1),_scene);

    const image img = ray_tracer.render_image();
    ppm_writer img_writer;

    img_writer.write(img,"../output.ppm");

    return 0;
}