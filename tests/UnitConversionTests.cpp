#include "GenericTestSuite.hpp"

template <class T>
class UnitConversionTests : public GenericTestSuite<T>
{
};

TYPED_TEST_SUITE(UnitConversionTests, Types);

//=============================================================================
// Tests: Base unit to all other units (1 unit of base)
//=============================================================================

TYPED_TEST(UnitConversionTests, BaseUnitToOtherUnits)
{
    using Traits = TestFixture::Traits;

    constexpr double baseValue = 1.0;
    TypeParam measurement      = this->asBase(baseValue);

    for (auto targetUnit : Traits::wellKnownUnits()) {
        if (targetUnit == Traits::baseUnits) {
            EXPECT_NEAR(measurement.value(targetUnit), baseValue, this->eps);
            return;
        }

        double converted = measurement.value(targetUnit);
        EXPECT_TRUE(std::isfinite(converted));
        // Temperature conversions can be zero, Just verify it's finite
        if constexpr (!std::is_same_v<TypeParam, Temperature>)
            EXPECT_GT(std::abs(converted), 0.0);
    }
}

//=============================================================================
// Tests: Round trip conversions from base unit
//=============================================================================

TYPED_TEST(UnitConversionTests, RoundTripFromBaseUnit)
{
    using Traits = TestFixture::Traits;

    std::vector testValues = {0.0, 1.0, -1.0, 0.5, 100.0};

    for (double original : testValues) {
        TypeParam originalMeasurement = this->asBase(original);
        for (auto targetUnit : Traits::wellKnownUnits()) {
            TypeParam roundTrip(targetUnit, originalMeasurement.value(targetUnit));
            EXPECT_NEAR(roundTrip.value(Traits::baseUnits), original, this->eps);
        }
    }
}

//=============================================================================
// Tests: Round trip conversions between all unit pairs
//=============================================================================

TYPED_TEST(UnitConversionTests, RoundTripBetweenAllUnits)
{
    using Traits = TestFixture::Traits;

    std::vector testValues = {0.0, 1.0, -1.0, 0.5, 100.0};

    for (double originalValue : testValues) {
        for (auto sourceUnit : Traits::wellKnownUnits()) {
            TypeParam source(sourceUnit, originalValue);
            for (auto targetUnit : Traits::wellKnownUnits()) {
                double backToSource = TypeParam(targetUnit, source.value(targetUnit))
                                          .value(sourceUnit);
                EXPECT_NEAR(backToSource, originalValue, this->eps);
            }
        }
    }
}

//=============================================================================
// Tests: Zero value in all units
//=============================================================================

TYPED_TEST(UnitConversionTests, ZeroInAllUnits)
{
    using Traits = TestFixture::Traits;

    for (auto unit : Traits::wellKnownUnits()) {
        TypeParam measurement(unit, 0.0);

        if constexpr (std::is_same_v<TypeParam, Temperature>) {
            if (unit == TemperatureUnits::Kelvin) {
                EXPECT_NEAR(measurement.value(TemperatureUnits::Celsius), -273.15, this->eps);
                EXPECT_NEAR(measurement.value(TemperatureUnits::Fahrenheit), -459.67, this->eps);
            } else if (unit == TemperatureUnits::Celsius) {
                EXPECT_NEAR(measurement.value(TemperatureUnits::Kelvin), 273.15, this->eps);
                EXPECT_NEAR(measurement.value(TemperatureUnits::Fahrenheit), 32.0, this->eps);
            } else if (unit == TemperatureUnits::Fahrenheit) {
                EXPECT_NEAR(measurement.value(TemperatureUnits::Celsius), -17.7777777778,
                            this->eps);
                EXPECT_NEAR(measurement.value(TemperatureUnits::Kelvin), 255.3722222222, this->eps);
            }
        } else {
            // For Angle and Distance, zero is zero in all units
            for (auto targetUnit : Traits::wellKnownUnits()) {
                EXPECT_NEAR(measurement.value(targetUnit), 0.0, this->eps);
            }
        }
    }
}

//=============================================================================
// Tests: Consistency across conversion paths
//=============================================================================

TYPED_TEST(UnitConversionTests, ConversionPathConsistency)
{
    using Traits = TestFixture::Traits;

    double testValue = 123.456;
    for (auto startUnit : Traits::wellKnownUnits()) {
        for (auto midUnit : Traits::wellKnownUnits()) {
            for (auto endUnit : Traits::wellKnownUnits()) {
                if (startUnit == endUnit)
                    continue;

                // Direct conversion
                TypeParam directStart(startUnit, testValue);
                double direct = directStart.value(endUnit);

                // Path through midUnit
                TypeParam viaMid(startUnit, testValue);
                double toMid = viaMid.value(midUnit);
                TypeParam fromMid(midUnit, toMid);
                double via = fromMid.value(endUnit);

                // For path consistency, use relative epsilon based on the magnitude
                double maxVal = std::max(std::abs(direct), std::abs(via));
                double eps    = 1e-9 * (1.0 + maxVal); // Scale with magnitude

                EXPECT_NEAR(direct, via, eps);
            }
        }
    }
}

//=============================================================================
// Tests: Inverse relationship (for linear scales)
//=============================================================================

TYPED_TEST(UnitConversionTests, InverseRelationship)
{
    using Traits = TestFixture::Traits;

    if constexpr (std::is_same_v<TypeParam, Temperature>) {
        GTEST_SKIP();
    } else {
        for (auto unitA : Traits::wellKnownUnits()) {
            for (auto unitB : Traits::wellKnownUnits()) {
                if (unitA == unitB)
                    continue;

                // Conversion factors should be inverses
                TypeParam oneInA(unitA, 1.0);
                double factorAToB = oneInA.value(unitB);

                TypeParam oneInB(unitB, 1.0);
                double factorBToA = oneInB.value(unitA);

                EXPECT_NEAR(factorAToB * factorBToA, 1.0, this->eps);
            }
        }
    }
}

//=============================================================================
// Tests: Specific value verifications
//=============================================================================

TYPED_TEST(UnitConversionTests, SpecificValueVerifications)
{
    if constexpr (std::is_same_v<TypeParam, Angle>) {
        // Base unit: Degrees

        // Degrees <-> Rotations
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 360.0).value(AngleUnits::Rotations), 1.0, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Rotations, 1.0).value(AngleUnits::Degrees), 360.0, this->eps);

        // Degrees <-> Radians
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 180.0).value(AngleUnits::Radians), M_PI, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Radians, M_PI).value(AngleUnits::Degrees), 180.0, this->eps);

        // Degrees <-> Gon
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 90.0).value(AngleUnits::Gon), 100.0, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Gon, 100.0).value(AngleUnits::Degrees), 90.0, this->eps);

        // Degrees <-> Artillery (6 degrees = 1 artillery)
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 6.0).value(AngleUnits::Artillery), 1.0, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Artillery, 1.0).value(AngleUnits::Degrees), 6.0, this->eps);

        // Degrees <-> SmallArtillery (6 degrees = 100 small artillery)
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 6.0).value(AngleUnits::SmallArtillery), 100.0,
                    this->eps);
        EXPECT_NEAR(Angle(AngleUnits::SmallArtillery, 100.0).value(AngleUnits::Degrees), 6.0,
                    this->eps);

        // Degrees <-> Minutes
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 1.0).value(AngleUnits::Minutes), 60.0, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Minutes, 60.0).value(AngleUnits::Degrees), 1.0, this->eps);

        // Degrees <-> Seconds
        EXPECT_NEAR(Angle(AngleUnits::Degrees, 1.0).value(AngleUnits::Seconds), 3600.0, this->eps);
        EXPECT_NEAR(Angle(AngleUnits::Seconds, 3600.0).value(AngleUnits::Degrees), 1.0, this->eps);

    } else if constexpr (std::is_same_v<TypeParam, Distance>) {
        // Base unit: Meters

        // Meters <-> Kilometers
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1000.0).value(DistanceUnits::Kilometers), 1.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Kilometers, 1.0).value(DistanceUnits::Meters), 1000.0,
                    this->eps);

        // Meters <-> Centimeters
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1.0).value(DistanceUnits::Centimeters), 100.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Centimeters, 100.0).value(DistanceUnits::Meters), 1.0,
                    this->eps);

        // Meters <-> Millimeters
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1.0).value(DistanceUnits::Millimeters), 1000.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Millimeters, 1000.0).value(DistanceUnits::Meters), 1.0,
                    this->eps);

        // Meters <-> Micrometers
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1.0).value(DistanceUnits::Micrometers),
                    1'000'000.0, this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Micrometers, 1'000'000.0).value(DistanceUnits::Meters),
                    1.0, this->eps);

        // Meters <-> Decimeters
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1.0).value(DistanceUnits::Decimeters), 10.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Decimeters, 10.0).value(DistanceUnits::Meters), 1.0,
                    this->eps);

        // Meters <-> Feet
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1.0).value(DistanceUnits::Feet),
                    3.280839895013123, this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Feet, 3.280839895013123).value(DistanceUnits::Meters),
                    1.0, this->eps);

        // Meters <-> Inches
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 0.0254).value(DistanceUnits::Inches), 1.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Inches, 1.0).value(DistanceUnits::Meters), 0.0254,
                    this->eps);

        // Meters <-> Miles
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1609.344).value(DistanceUnits::Miles), 1.0,
                    this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::Miles, 1.0).value(DistanceUnits::Meters), 1609.344,
                    this->eps);

        // Meters <-> NauticalMiles
        EXPECT_NEAR(Distance(DistanceUnits::Meters, 1852.0).value(DistanceUnits::NauticalMiles),
                    1.0, this->eps);
        EXPECT_NEAR(Distance(DistanceUnits::NauticalMiles, 1.0).value(DistanceUnits::Meters),
                    1852.0, this->eps);

    } else if constexpr (std::is_same_v<TypeParam, Temperature>) {
        // Base unit: Celsius

        // Celsius <-> Fahrenheit
        EXPECT_NEAR(Temperature(TemperatureUnits::Celsius, 0.0).value(TemperatureUnits::Fahrenheit),
                    32.0, this->eps);
        EXPECT_NEAR(
            Temperature(TemperatureUnits::Fahrenheit, 32.0).value(TemperatureUnits::Celsius), 0.0,
            this->eps);

        EXPECT_NEAR(
            Temperature(TemperatureUnits::Celsius, 100.0).value(TemperatureUnits::Fahrenheit),
            212.0, this->eps);
        EXPECT_NEAR(
            Temperature(TemperatureUnits::Fahrenheit, 212.0).value(TemperatureUnits::Celsius),
            100.0, this->eps);

        EXPECT_NEAR(
            Temperature(TemperatureUnits::Celsius, -40.0).value(TemperatureUnits::Fahrenheit),
            -40.0, this->eps);
        EXPECT_NEAR(
            Temperature(TemperatureUnits::Fahrenheit, -40.0).value(TemperatureUnits::Celsius),
            -40.0, this->eps);

        // Celsius <-> Kelvin
        EXPECT_NEAR(Temperature(TemperatureUnits::Celsius, 0.0).value(TemperatureUnits::Kelvin),
                    273.15, this->eps);
        EXPECT_NEAR(Temperature(TemperatureUnits::Kelvin, 273.15).value(TemperatureUnits::Celsius),
                    0.0, this->eps);

        EXPECT_NEAR(Temperature(TemperatureUnits::Celsius, 100.0).value(TemperatureUnits::Kelvin),
                    373.15, this->eps);
        EXPECT_NEAR(Temperature(TemperatureUnits::Kelvin, 373.15).value(TemperatureUnits::Celsius),
                    100.0, this->eps);

        EXPECT_NEAR(Temperature(TemperatureUnits::Celsius, -273.15).value(TemperatureUnits::Kelvin),
                    0.0, this->eps);
        EXPECT_NEAR(Temperature(TemperatureUnits::Kelvin, 0.0).value(TemperatureUnits::Celsius),
                    -273.15, this->eps);
    }
}

//=============================================================================
// Tests: Negative value conversions
//=============================================================================

TYPED_TEST(UnitConversionTests, NegativeValues)
{
    using Traits = TestFixture::Traits;

    std::vector negativeValues = {-1.0, -10.0, -180.0};

    if constexpr (std::is_same_v<TypeParam, Temperature>) {
        negativeValues = {-40.0, -100.0, -273.15};
    }

    for (double negativeValue : negativeValues) {
        for (auto sourceUnit : Traits::wellKnownUnits()) {
            TypeParam source(sourceUnit, negativeValue);

            for (auto targetUnit : Traits::wellKnownUnits()) {
                double converted = source.value(targetUnit);

                TypeParam back(targetUnit, converted);
                double backToOriginal = back.value(sourceUnit);

                EXPECT_NEAR(backToOriginal, negativeValue, this->eps);
            }
        }
    }
}

//=============================================================================
// Tests: Large value conversions
//=============================================================================

TYPED_TEST(UnitConversionTests, LargeValues)
{
    using Traits = TestFixture::Traits;

    std::vector largeValues = {1000.0, 10000.0, 100000.0};

    for (double largeValue : largeValues) {
        for (auto sourceUnit : Traits::wellKnownUnits()) {
            TypeParam source(sourceUnit, largeValue);

            for (auto targetUnit : Traits::wellKnownUnits()) {
                double converted = source.value(targetUnit);
                EXPECT_TRUE(std::isfinite(converted));

                TypeParam back(targetUnit, converted);
                double backToOriginal = back.value(sourceUnit);
                EXPECT_NEAR(backToOriginal, largeValue, this->eps * largeValue);
            }
        }
    }
}
