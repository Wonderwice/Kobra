#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "renderer/raytracer.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"

using namespace cobra;

int main(){
    const size_t image_width = 1920;
    const double aspect_ratio = 16./9.;

    raytracer ray_tracer(1, 1, camera(image_width,aspect_ratio,2,1), scene());

    const image img = ray_tracer.render_image();
    ppm_writer img_writer;

    img_writer.write(img,"../output.ppm");

    return 0;
}