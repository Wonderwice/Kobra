#pragma once
#include <vector>
#include "math/vec3.h"

namespace cobra
{
    /**
     * @brief Represents a 2D image with pixels stored as vec3 color values.
     * 
     * The image stores pixel colors in row-major order internally.
     */
    class image
    {
    private:
        std::vector<vec3> img_buffer; ///< Flat buffer holding color data of all pixels
        const size_t width;            ///< Image width in pixels
        const size_t height;           ///< Image height in pixels

        /**
         * @brief Helper to compute the linear index in img_buffer from (row, col).
         * @param row Row index of the pixel.
         * @param col Column index of the pixel.
         * @return Index in the img_buffer vector.
         */
        size_t index(const size_t row, const size_t col) const;

    public:
        /**
         * @brief Constructs an image with given width and height.
         * Initializes all pixels to zero (black).
         * @param width Width of the image.
         * @param height Height of the image.
         */
        image(const size_t width, const size_t height);

        /**
         * @brief Destructor.
         */
        ~image();

        /**
         * @brief Get the width of the image.
         * @return Image width in pixels.
         */
        size_t get_width() const;

        /**
         * @brief Get the height of the image.
         * @return Image height in pixels.
         */
        size_t get_height() const;

        /**
         * @brief Set the color of a pixel at (row, col).
         * @param row Row index of the pixel.
         * @param col Column index of the pixel.
         * @param color The color value to set.
         */
        void set_pixel(const size_t row, const size_t col, const vec3& color);

        /**
         * @brief Get the color of a pixel at (row, col).
         * @param row Row index of the pixel.
         * @param col Column index of the pixel.
         * @return Color value of the pixel.
         */
        vec3 get_pixel(const size_t row, const size_t col) const;
    };
}
