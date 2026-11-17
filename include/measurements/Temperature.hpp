#pragma once

#include "prototypes/Measurement.hpp"
#include "units/TemperatureUnits.hpp"

class Temperature : public Measurement<Temperature, TemperatureUnits>
{
public:
    using Measurement::Measurement;
};
