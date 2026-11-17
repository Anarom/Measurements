#pragma once

#include "measurements/prototypes/MeasurementUnits.hpp"

#include <array>

struct TimeUnits
{
    TimeUnits() = delete;

    class Type : public MeasurementUnits<>
    {
    public:
        using MeasurementUnits::MeasurementUnits;
    };

    // NOLINTBEGIN(readability-identifier-naming)
    static constexpr Type Weeks{"Weeks", {.multiplierToBase = 604800}};
    static constexpr Type Days{"Days", {.multiplierToBase = 86400}};
    static constexpr Type Hours{"Hours", {.multiplierToBase = 3600}};
    static constexpr Type Minutes{"Minutes", {.multiplierToBase = 60}};
    static constexpr Type Seconds{"Seconds", {}};
    static constexpr Type Milliseconds{"Milliseconds", {.multiplierToBase = 0.001}};
    static constexpr Type Microseconds{"Microseconds", {.multiplierToBase = 0.000001}};
    static constexpr Type Nanoseconds{"Nanoseconds", {.multiplierToBase = 0.000000001}};
    // NOLINTEND(readability-identifier-naming)
};

template <>
struct UnitTraits<TimeUnits>
{
    static constexpr auto wellKnownUnits()
    {
        return std::array{
            TimeUnits::Weeks,        TimeUnits::Days,        TimeUnits::Hours,
            TimeUnits::Minutes,      TimeUnits::Seconds,     TimeUnits::Milliseconds,
            TimeUnits::Microseconds, TimeUnits::Nanoseconds,
        };
    }

    static constexpr auto baseUnits = TimeUnits::Seconds;
};
