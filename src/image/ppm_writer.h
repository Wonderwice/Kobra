#pragma once
#include "image/image_writer.h"

namespace cobra
{
    /**
     * @brief Concrete image writer for the PPM image format.
     * 
     * Implements writing images in the plain PPM (Portable Pixmap) format,
     * supporting output to both std::ostream and file paths.
     */
    class ppm_writer : public image_writer
    {
    public:
        /**
         * @brief Constructs a new ppm_writer object.
         */
        ppm_writer();

        /**
         * @brief Destructor for ppm_writer.
         */
        ~ppm_writer();

        /**
         * @brief Writes the given image to an output stream in PPM format.
         * 
         * @param image The image to write.
         * @param os The output stream to write to.
         * @return true if writing succeeds, false otherwise.
         */
        bool write(const image &image, std::ostream &os) const override;

        /**
         * @brief Writes the given image to a file in PPM format.
         * 
         * @param image The image to write.
         * @param filename The file path to save the image.
         * @return true if writing succeeds, false otherwise.
         */
        bool write(const image &image, std::string filename) const override;

        /**
         * @brief Convert values from linear space to gamma space.
         * @param The value to convert.
         * @return The converted value.
         */
        double linear_to_gamma(double linear_component) const;
    };
}
