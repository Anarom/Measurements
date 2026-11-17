#pragma once

#include <gtest/gtest.h>
#include <measurements/Angle.hpp>
#include <measurements/Distance.hpp>
#include <measurements/Temperature.hpp>
#include <measurements/Time.hpp>

template <class T>
class GenericTestSuite : public ::testing::Test
{
public:
    using UnitsType = T::Units::Type;
    using Traits    = UnitTraits<typename T::Units>;

    static constexpr T asBase(double amount)
    {
        return T{UnitTraits<typename T::Units>::baseUnits, amount};
    }

    static constexpr double toBase(T value) { return value.value(Traits::baseUnits); }

    constexpr static double eps = 1e-10;
};

using Types = ::testing::Types<Angle, Distance, Temperature, Time>;
