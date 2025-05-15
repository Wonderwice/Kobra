#include "image/ppm_writer.h"
#include <fstream>

namespace cobra
{
    ppm_writer::ppm_writer() {}

    ppm_writer::~ppm_writer() {}

    bool ppm_writer::write(const image &img, std::ostream &os) const
    {
        const size_t width = img.get_width();
        const size_t height = img.get_height();

        os << "P3\n"
           << width << " " << height << "\n255\n";
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                vec3 color = img.get_pixel(x, y);
                int r = std::min(255, std::max(0, static_cast<int>(color.x() * 255.0)));
                int g = std::min(255, std::max(0, static_cast<int>(color.y() * 255.0)));
                int b = std::min(255, std::max(0, static_cast<int>(color.z() * 255.0)));
                os << r << " " << g << " " << b << " ";
            }
            os << "\n";
        }

        return true;
    }

    bool ppm_writer::write(const image &img, std::string filename) const
    {
        std::ofstream ofs(filename);
        if (!ofs.is_open())
            return false;

        return write(img, ofs);
    }

}
