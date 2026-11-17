#pragma once

#include "measurements/prototypes/MeasurementUnits.hpp"

#include <array>
#include <numbers>

struct AngleUnits
{
    AngleUnits() = delete;

    class Type : public MeasurementUnits<>
    {
    public:
        using MeasurementUnits::MeasurementUnits;
    };

    // NOLINTBEGIN(readability-identifier-naming)
    static constexpr Type Rotations{"Rotations", {.multiplierToBase = 360}};
    static constexpr Type Degrees{"Degrees", {}};
    static constexpr Type Radians{"Radians", {.multiplierToBase = 180 / std::numbers::pi}};
    static constexpr Type Gon{"Gon", {.multiplierToBase = 0.9}};
    static constexpr Type Artillery{"Large Artillery", {.multiplierToBase = 6}};
    static constexpr Type SmallArtillery{"Small Artillery", {.multiplierToBase = 0.06}};
    static constexpr Type Minutes{"Minutes", {.multiplierToBase = 1. / 60.}};
    static constexpr Type Seconds{"Seconds", {.multiplierToBase = 1. / 3600.}};
    // NOLINTEND(readability-identifier-naming)
};

template <>
struct UnitTraits<AngleUnits>
{
    static constexpr auto wellKnownUnits()
    {
        return std::array{
            AngleUnits::Rotations, AngleUnits::Degrees,   AngleUnits::Radians,
            AngleUnits::Gon,       AngleUnits::Artillery, AngleUnits::SmallArtillery,
            AngleUnits::Minutes,   AngleUnits::Seconds,
        };
    }

    static constexpr auto baseUnits = AngleUnits::Degrees;
};
