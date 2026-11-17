#pragma once

#include <algorithm>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

#if __cplusplus >= 202302L
#    define CONSTEXPR_23 constexpr
#else
#    define CONSTEXPR_23
#endif

#if __cplusplus >= 202603L
#    define CONSTEXPR_26 constexpr
#else
#    define CONSTEXPR_26
#endif

CONSTEXPR_23 inline bool fuzzyCompare(double p1, double p2) noexcept
{
    if (p1 == p2)
        return true;
    if (!std::isfinite(p1) || !std::isfinite(p2))
        return false;
    return std::abs(p1 - p2) <= 1e-12 * std::min(std::abs(p1), std::abs(p2));
}

inline double roundedDouble(double value, uint8_t roundingPrecision)
{
    // no-op beyond double precision
    if (roundingPrecision > 15)
        return value;

    double scale        = std::pow(10.0, roundingPrecision);
    double inverseScale = 1.0 / scale;

    double scaled        = value * scale;
    double roundedScaled = std::round(scaled);

    // as in fuzzyCompare
    constexpr double relativeEps = 1e-12;
    double tolerance             = relativeEps * std::abs(scaled);
    double distToClosestHalfInt  = std::abs(scaled - std::floor(scaled) - 0.5);

    if (distToClosestHalfInt < tolerance && tolerance < 0.5) {
        int sign      = value >= 0.0 ? 1 : -1;
        roundedScaled = std::round(scaled + sign * 2.0 * tolerance);
    }
    return roundedScaled * inverseScale;
}

/**
 * @brief A physical quantity, expressed in the units category @p U.
 *
 * @tparam D The concrete derived class (CRTP). Arithmetic operators return @p D rather than @p
 * Measurement, so that adding two @p Distance objects yields a @p Distance.
 * @tparam U The units category (e.g. @p DistanceUnits). Selects the conversion
 *           parameters and the hub unit for the stored value.
 *
 * @details The numeric value is stored as a single @c double in the hub unit of
 * category @p U (see @ref MeasurementUnits). All derived types are trivially
 * copyable and occupy @c sizeof(double) on the stack, so instances can be passed
 * by value.
 *
 * @details All arithmetic and comparison operators require operands of the
 * same category @p U, so it is impossible to add angular seconds to
 * seconds of time, even though both are spelled "seconds".
 *
 * @details @ref operator== and @ref operator<=> compare exactly, which makes
 * derived types safe to use in ordered and hashed containers. After unit
 * conversions exact equality is rarely achieved; use @ref fuzzyEqual when a
 * tolerance-based comparison is intended.
 */
template <class D, class U>
class Measurement
{
public:
    using Derived = D;
    using Units   = U;

    /// @brief Constructs a measurement equal to zero in the hub unit.
    constexpr Measurement() = default;

    /// @brief Constructs a measurement equal to @p value expressed in @p units.
    template <std::convertible_to<double> T>
    constexpr Measurement(const U::Type& units, T value)
        : m_valueInBaseUnits(units.conversion.toBaseUnits(value))
    {
    }

    /**
     * @brief The value in @p units, rounded to @p roundingPrecision decimal digits.
     * @sa roundedDouble()
     */
    CONSTEXPR_26 double roundedValue(const U::Type& units, uint8_t roundingPrecision) const
    {
        return roundedDouble(value(units), roundingPrecision);
    }

    constexpr double value(const U::Type& units) const noexcept
    {
        return units.conversion.fromBaseUnits(m_valueInBaseUnits);
    }

    /// @brief A new measurement equal to the fractional part of original in @p units.
    CONSTEXPR_23 Derived normalized(const U::Type& units) const noexcept
    {
        double v = value(units);
        return Derived(units, v - std::trunc(v));
    }

    CONSTEXPR_23 bool fuzzyEqual(const Measurement& rhs) const
    {
        return fuzzyCompare(m_valueInBaseUnits, rhs.m_valueInBaseUnits);
    }

    constexpr bool operator==(const Measurement&) const noexcept = default;

    constexpr auto operator<=>(const Measurement& rhs) const noexcept
    {
        return m_valueInBaseUnits <=> rhs.m_valueInBaseUnits;
    }

    constexpr friend Derived operator*(double modifier, const Derived& measurement) noexcept
    {
        return measurement * modifier;
    }

    constexpr Derived& operator+=(const Derived& rhs) noexcept
    {
        m_valueInBaseUnits += rhs.m_valueInBaseUnits;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator-=(const Derived& rhs) noexcept
    {
        m_valueInBaseUnits -= rhs.m_valueInBaseUnits;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator*=(double rhs) noexcept
    {
        m_valueInBaseUnits *= rhs;
        return static_cast<Derived&>(*this);
    }

    /// @throws std::domain_error if @p rhs is zero.
    constexpr Derived& operator/=(double rhs)
    {
        if (rhs == 0)
            throw std::domain_error("Division by zero");

        m_valueInBaseUnits /= rhs;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator-() const noexcept
    {
        Derived temp(static_cast<const Derived&>(*this));
        temp.m_valueInBaseUnits = -temp.m_valueInBaseUnits;
        return temp;
    }

    constexpr Derived operator+(const Derived& rhs) const noexcept
    {
        Derived temp(static_cast<const Derived&>(*this));
        temp += rhs;
        return temp;
    }

    constexpr Derived operator-(const Derived& rhs) const noexcept
    {
        Derived temp(static_cast<const Derived&>(*this));
        temp -= rhs;
        return temp;
    }

    /// @throws std::domain_error if @p rhs is zero in hub units.
    constexpr double operator/(const Derived& rhs) const
    {
        if (rhs.m_valueInBaseUnits == 0)
            throw std::domain_error("Division by zero");

        return m_valueInBaseUnits / rhs.m_valueInBaseUnits;
    }

    constexpr Derived operator*(double rhs) const noexcept
    {
        Derived temp(static_cast<const Derived&>(*this));
        temp *= rhs;
        return temp;
    }

    /// @throws std::domain_error if @p rhs is zero.
    constexpr Derived operator/(double rhs) const
    {
        if (rhs == 0)
            throw std::domain_error("Division by zero");

        Derived temp(static_cast<const Derived&>(*this));
        temp /= rhs;
        return temp;
    }

private:
    double m_valueInBaseUnits = 0;
};

//================================================================================================//
template <class T>
concept IsMeasurement = requires(std::remove_cvref_t<T> t) {
    []<class D, class U>(const Measurement<D, U>&) {}(t);
};
