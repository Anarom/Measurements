// SPDX-License-Identifier: MIT
#pragma once
#include <string_view>

namespace conversions {
/**
 * @brief Linear conversion to the hub unit: @c value *  @c multiplierToBase.
 * @details The hub unit of a category uses @c multiplierToBase == 1  (meters, degrees, seconds).
 * Other units in the same category store their own multiplier relative to the hub.
 */
struct LinearConversion
{
    double multiplierToBase = 1;

    bool operator==(const LinearConversion& rhs) const = default;

    constexpr double fromBaseUnits(double amount) const { return amount / multiplierToBase; }

    constexpr double toBaseUnits(double amount) const { return amount * multiplierToBase; }
};

/**
 * @brief Two step conversion to the hub unit: (@c value + @c additionToBase) * @c multiplierToBase.
 * @details Used for Celsius <-> Kelvin conversion.
 */
struct TwoStepConversion
{
    double multiplierToBase = 1;
    double additionToBase   = 0;

    bool operator==(const TwoStepConversion& rhs) const = default;

    constexpr double fromBaseUnits(double amount) const
    {
        return amount / multiplierToBase - additionToBase;
    }

    constexpr double toBaseUnits(double amount) const
    {
        return (amount + additionToBase) * multiplierToBase;
    }
};

} // namespace conversions

/**
 * @brief Provides array<T::Type> of wellKnownUnits and T::TYpe units which act as hub for
 * conversions.
 */
template <class>
struct UnitTraits;

/**
 * @brief Base class for a units category. Each instance represents one unit.
 *
 * @tparam Conversion Type providing @c constexpr @ref toBaseUnits and
 *         @ref fromBaseUnits. Defaults to a linear conversion.
 *
 * @details Units are usually declared as @c static @c constexpr instances.
 * @ref name  @b must come from a bare string literal, since
 * @c std::string is not @c constexpr, and only such instances can be used in
 * @c constexpr @ref Measurement constructors. A unit can also be constructed
 * at runtime from arbitrary conversion parameters; the result is usable with
 * @ref Measurement but not in @c constexpr contexts. @p Time class uses this as implementation
 * detail to represent each @c std::chrono::duration specialization as a unit.
 */
template <class Conversion = conversions::LinearConversion>
struct MeasurementUnits
{
    constexpr explicit MeasurementUnits(std::string_view name, Conversion conversion)
        : name(name)
        , conversion(conversion)
    {
    }

    bool operator==(const MeasurementUnits& rhs) const = default;

    std::string_view name; ///< Unit name for display. Must outlive the instance.
    Conversion conversion; ///< Parameters of the conversion to and from the hub unit.
};
