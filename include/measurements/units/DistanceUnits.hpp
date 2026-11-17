#pragma once

#include "measurements/prototypes/MeasurementUnits.hpp"

#include <array>

struct DistanceUnits
{
    DistanceUnits() = delete;

    class Type : public MeasurementUnits<>
    {
    public:
        using MeasurementUnits::MeasurementUnits;
    };

    // NOLINTBEGIN(readability-identifier-naming)
    static constexpr Type NauticalMiles{"Nautical miles", {.multiplierToBase = 1852}};
    static constexpr Type Miles{"Statue miles", {.multiplierToBase = 1609.344}};
    static constexpr Type Kilometers{"Kilometers", {.multiplierToBase = 1000}};
    static constexpr Type Meters{"Meters", {}};
    static constexpr Type Yards{"Yards", {.multiplierToBase = 0.9144}};
    static constexpr Type Feet{"Feet", {.multiplierToBase = 0.3048}};
    static constexpr Type Decimeters{"Decimeter", {.multiplierToBase = 0.1}};
    static constexpr Type Inches{"Inches", {.multiplierToBase = 0.0254}};
    static constexpr Type Centimeters{"Centimeters", {.multiplierToBase = 0.01}};
    static constexpr Type Millimeters{"Millimeters", {.multiplierToBase = 0.001}};
    static constexpr Type Micrometers{"Micrometers", {.multiplierToBase = 0.000001}};
    // NOLINTEND(readability-identifier-naming)
};

template <>
struct UnitTraits<DistanceUnits>
{
    static constexpr auto wellKnownUnits()
    {
        return std::array{
            DistanceUnits::NauticalMiles, DistanceUnits::Miles,       DistanceUnits::Kilometers,
            DistanceUnits::Meters,        DistanceUnits::Yards,       DistanceUnits::Feet,
            DistanceUnits::Decimeters,    DistanceUnits::Inches,      DistanceUnits::Centimeters,
            DistanceUnits::Millimeters,   DistanceUnits::Micrometers,
        };
    }

    static constexpr auto baseUnits = DistanceUnits::Meters;
};
