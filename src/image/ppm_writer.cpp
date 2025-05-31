#include "image/ppm_writer.h"
#include <fstream>
#include <cmath>
#include "image_writer.h"

namespace cobra
{
    ppm_writer::ppm_writer() {}

    ppm_writer::~ppm_writer() {}

    double ppm_writer::linear_to_gamma(double linear_component) const
    {
        if (linear_component > 0)
            return std::sqrt(linear_component);

        return 0;
    }

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
                vec3 color = img.get_pixel(y, x);
                auto r = color.x();
                auto g = color.y();
                auto b = color.z();

                if (r != r)
                    r = 0.0;
                if (g != g)
                    g = 0.0;
                if (b != b)
                    b = 0.0;

                r = std::min(255, std::max(0, static_cast<int>(linear_to_gamma(r) * 255.0)));
                g = std::min(255, std::max(0, static_cast<int>(linear_to_gamma(g) * 255.0)));
                b = std::min(255, std::max(0, static_cast<int>(linear_to_gamma(b) * 255.0)));
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
