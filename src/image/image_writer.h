#pragma once

#include <string>
#include <iostream>
#include "image/image.h"

namespace cobra
{
    /**
     * @brief Abstract base class for image writers.
     * 
     * Provides an interface to write an image to either an output stream
     * or directly to a file specified by a filename.
     * 
     * This class cannot be instantiated directly because it contains pure virtual methods.
     * Derived classes must implement the write methods to define specific image output formats.
     */
    class image_writer
    {
    public:
        /**
         * @brief Default constructor.
         */
        image_writer() {}

        /**
         * @brief Virtual destructor.
         * 
         * Declared pure virtual to make the class abstract,
         * but must have a definition to allow proper cleanup
         * of derived classes.
         */
        virtual ~image_writer() = 0;

        /**
         * @brief Write an image to the provided output stream.
         * 
         * @param image The image object to be written.
         * @param os The output stream to write the image data to.
         * @return true if writing succeeded, false otherwise.
         */
        virtual bool write(const image &image, std::ostream &os) const = 0;

        /**
         * @brief Write an image to a file.
         * 
         * @param image The image object to be written.
         * @param filename The path to the file where the image will be saved.
         * @return true if writing succeeded, false otherwise.
         */
        virtual bool write(const image &image, std::string filename) const = 0;
    };

    inline image_writer::~image_writer()
    {
    }
}
