#pragma once

#include "prototypes/Measurement.hpp"
#include "units/DistanceUnits.hpp"

class Distance : public Measurement<Distance, DistanceUnits>
{
public:
    using Measurement::Measurement;
};
