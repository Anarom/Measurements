#pragma once

#include "prototypes/Measurement.hpp"
#include "units/TimeUnits.hpp"

#include <chrono>

template <class T>
concept StdChronoDurationConcept = requires(T t) {
    []<class Rep, class Period>(std::chrono::duration<Rep, Period>&) {}(t);
};

class Time : public Measurement<Time, TimeUnits>
{
public:
    using Measurement::Measurement;

    template <StdChronoDurationConcept T>
    constexpr Time(const T& duration) : Time(stdUnits<T>(), duration.count())
    {
    }

    template <StdChronoDurationConcept T>
    constexpr Time& operator=(const T& duration) noexcept
    {
        *this = Time(duration);
        return *this;
    }

    template <StdChronoDurationConcept T>
    constexpr explicit operator T() const
    {
        return T(static_cast<T::rep>(value(stdUnits<T>())));
    }

private:
    template <StdChronoDurationConcept T>
    friend constexpr bool operator==(const Time& time, const T& duration);

    template <StdChronoDurationConcept T>
    friend constexpr auto operator<=>(const Time& time, const T& duration);

    template <StdChronoDurationConcept T>
    static constexpr TimeUnits::Type stdUnits()
    {
        return TimeUnits::Type("", {
                                       .multiplierToBase = static_cast<double>(T::period::num)
                                                           / T::period::den,
                                   });
    }
};

template <StdChronoDurationConcept T>
constexpr bool operator==(const Time& time, const T& duration)
{
    return time.value(Time::stdUnits<T>()) == static_cast<double>(duration.count());
}

template <StdChronoDurationConcept T>
constexpr auto operator<=>(const Time& time, const T& duration)
{
    return time.value(Time::stdUnits<T>()) <=> static_cast<double>(duration.count());
}
