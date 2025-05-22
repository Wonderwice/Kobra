#pragma once

#include "cobra.h"

namespace cobra
{
    /**
     * @class interval
     * @brief Represents a 1D interval on the real number line.
     *
     * Used for bounding ranges, such as in axis-aligned bounding boxes (AABB) or for clamping values.
     * An interval can be empty or cover the entire real line.
     */
    class interval
    {
    public:
        double min; ///< Minimum value of the interval.
        double max; ///< Maximum value of the interval.

        /**
         * @brief Default constructor: creates an empty interval.
         */
        interval() : min(+infinity), max(-infinity) {}

        /**
         * @brief Constructs an interval from a minimum and a maximum.
         * @param min Minimum value.
         * @param max Maximum value.
         */
        interval(double min, double max) : min(min), max(max) {}

        /**
         * @brief Constructs the smallest interval that contains both input intervals.
         * @param a First interval.
         * @param b Second interval.
         */
        interval(const interval &a, const interval &b)
        {
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        /**
         * @brief Returns the size (length) of the interval.
         * @return max - min.
         */
        double size() const
        {
            return max - min;
        }

        /**
         * @brief Checks if a value is within the interval (inclusive).
         * @param x The value to check.
         * @return True if min <= x <= max.
         */
        bool contains(double x) const
        {
            return min <= x && x <= max;
        }

        /**
         * @brief Checks if a value is strictly within the interval (exclusive).
         * @param x The value to check.
         * @return True if min < x < max.
         */
        bool surrounds(double x) const
        {
            return min < x && x < max;
        }

        /**
         * @brief Clamps a value to be within the interval.
         * @param x The value to clamp.
         * @return x clamped between min and max.
         */
        double clamp(double x) const
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }

        /**
         * @brief Expands the interval by a total of delta (half on each side).
         * @param delta Total amount to expand the interval.
         * @return A new interval expanded by delta.
         */
        interval expand(double delta) const
        {
            auto padding = delta / 2;
            return interval(min - padding, max + padding);
        }

        /// A constant representing the empty interval.
        static const interval empty;

        /// A constant representing the full universe interval (-∞, +∞).
        static const interval universe;
    };

    /// Definition of the empty interval constant.
    inline const interval interval::empty = interval(+infinity, -infinity);

    /// Definition of the universe interval constant.
    inline const interval interval::universe = interval(-infinity, +infinity);

    /**
     * @brief Shifts an interval by a constant displacement.
     * @param ival The interval to shift.
     * @param displacement The amount to shift.
     * @return A new interval shifted by the given amount.
     */
    inline interval operator+(const interval &ival, double displacement)
    {
        return interval(ival.min + displacement, ival.max + displacement);
    }

    /**
     * @brief Shifts an interval by a constant displacement (commutative overload).
     * @param displacement The amount to shift.
     * @param ival The interval to shift.
     * @return A new interval shifted by the given amount.
     */
    inline interval operator+(double displacement, const interval &ival)
    {
        return ival + displacement;
    }
}
