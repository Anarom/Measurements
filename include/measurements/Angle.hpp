#pragma once

#include "prototypes/Measurement.hpp"
#include "units/AngleUnits.hpp"

#include <algorithm>

class Angle : public Measurement<Angle, AngleUnits>
{
public:
    using Measurement::Measurement;

    /**
     * @brief Shortest arc length between reference angle and @p other.
     * @details Can be negative in hub units, for example signedArc from 10 deg to 5 deg will be -5
     * deg.
     */
    CONSTEXPR_23 Angle signedArc(Angle other) const
    {
        // https://stackoverflow.com/a/2007355
        auto d1 = (other - *this).normalized(AngleUnits::Rotations);
        auto d2 = d1 + Angle{AngleUnits::Rotations, 1};
        auto d3 = d1 - Angle{AngleUnits::Rotations, 1};

        return std::min({d1, d2, d3}, [](const Angle& left, const Angle& right) {
            return std::abs(left.value(AngleUnits::Degrees))
                   < std::abs(right.value(AngleUnits::Degrees));
        });
    }

    CONSTEXPR_26 double sin() const { return std::sin(value(AngleUnits::Radians)); }

    CONSTEXPR_26 double cos() const { return std::cos(value(AngleUnits::Radians)); }

    CONSTEXPR_26 double tan() const { return std::tan(value(AngleUnits::Radians)); }

    CONSTEXPR_26 double cot() const { return 1.0 / tan(); }
};
