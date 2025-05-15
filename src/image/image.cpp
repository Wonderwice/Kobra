#include "image/image.h"
#include "image.h"
namespace cobra
{
    size_t image::index(const size_t row, const size_t col) const
    {
        return row * width + col;
    }

    image::image(const size_t width, const size_t height) : img_buffer(width * height), width(width), height(height)
    {
    }

    image::~image()
    {
    }

    size_t image::get_width() const
    {
        return width;
    }

    size_t image::get_height() const
    {
        return height;
    }

    void image::set_pixel(const size_t row, const size_t col, const vec3 &color)
    {
        img_buffer[index(row, col)] = color;
    }

    vec3 cobra::image::get_pixel(const size_t row, const size_t col) const
    {
        return img_buffer[index(row, col)];
    }
}
