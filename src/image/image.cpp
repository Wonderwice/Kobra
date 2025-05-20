#include "image/image.h"
namespace cobra
{
    

    image::image(const size_t width, const size_t height) : img_buffer(width * height), width(width), height(height)
    {
    }

    image::~image()
    {
    }

}
