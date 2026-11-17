target_sources(
    ${PROJECT_NAME}
    PUBLIC
    FILE_SET HEADERS
        FILES
            measurements/Angle.hpp
            measurements/Distance.hpp
            measurements/prototypes/Measurement.hpp
            measurements/prototypes/MeasurementUnits.hpp
            measurements/Temperature.hpp
            measurements/Time.hpp
            measurements/units/AngleUnits.hpp
            measurements/units/DistanceUnits.hpp
            measurements/units/TemperatureUnits.hpp
            measurements/units/TimeUnits.hpp)
