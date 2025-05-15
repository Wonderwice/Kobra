#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "renderer/raytracer.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"

using namespace cobra;

int main(){
    const size_t image_width = 256;
    const size_t image_height = 256;

    raytracer ray_tracer(image_width, image_height, 1, 1, camera(), scene());

    const image img = ray_tracer.render_image();
    ppm_writer img_writer;

    img_writer.write(img,"../output.ppm");

    return 0;
}