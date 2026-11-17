#pragma once

#include "measurements/prototypes/MeasurementUnits.hpp"

#include <array>

struct TemperatureUnits
{
    TemperatureUnits() = delete;

    class Type : public MeasurementUnits<conversions::TwoStepConversion>
    {
    public:
        using MeasurementUnits::MeasurementUnits;
    };

    // NOLINTBEGIN(readability-identifier-naming)
    static constexpr Type Celsius{"Celsius", {}};
    static constexpr Type Fahrenheit{
        "Fahrenheit", {.multiplierToBase = 1. / 1.8, .additionToBase = -32}
    };
    static constexpr Type Kelvin{
        "Kelvin", {.multiplierToBase = 1, .additionToBase = -273.15}
        // NOLINTEND(readability-identifier-naming)
    };
};

template <>
struct UnitTraits<TemperatureUnits>
{
    static constexpr auto wellKnownUnits()
    {
        return std::array{
            TemperatureUnits::Celsius,
            TemperatureUnits::Fahrenheit,
            TemperatureUnits::Kelvin,
        };
    }

    static constexpr auto baseUnits = TemperatureUnits::Celsius;
};
