// SPDX-License-Identifier: MIT
#include "GenericTestSuite.hpp"

template <class T>
class GenericMethodsTests : public GenericTestSuite<T>
{
};

TYPED_TEST_SUITE(GenericMethodsTests, Types);

//=============================================================================
// Tests: Constructor
//=============================================================================

template <class T>
concept has_default_constructor = std::is_default_constructible_v<T>;

template <class T>
concept has_value_constructor = requires(typename T::Units::Type unit, double value) {
    T{unit, value};
};

TYPED_TEST(GenericMethodsTests, DefaultConstructorExists)
{
    EXPECT_TRUE(has_default_constructor<TypeParam>);
}

TYPED_TEST(GenericMethodsTests, DefaultConstructorCreatesZeroInBaseUnits)
{
    EXPECT_NEAR(TypeParam().value(TestFixture::Traits::baseUnits), 0.0, this->eps);
}

TYPED_TEST(GenericMethodsTests, ValueConstructorExists)
{
    EXPECT_TRUE(has_value_constructor<TypeParam>);
}

TYPED_TEST(GenericMethodsTests, ValueConstructorIsValid)
{
    using Traits = TestFixture::Traits;

    std::vector<double> testValues = {0, 1, 0.000000001, 10000, -1, -0.00000001, -10000};

    for (auto unit : Traits::wellKnownUnits()) {
        for (auto value : testValues)
            EXPECT_NEAR(TypeParam(unit, value).value(unit), value, this->eps);
    }
}

//=============================================================================
// Tests: base units are used for storage
//=============================================================================
TYPED_TEST(GenericMethodsTests, BaseUnitsUsedForStorage)
{
    auto value = TypeParam(TestFixture::Traits::baseUnits, 1);
    EXPECT_EQ(*reinterpret_cast<const double*>(&value), 1);
}

//=============================================================================
// Tests: normalized() method
//=============================================================================
TYPED_TEST(GenericMethodsTests, NormalizedBasicTests)
{
    using Traits = TestFixture::Traits;

    EXPECT_EQ(this->asBase(1).normalized(Traits::baseUnits), this->asBase(0));
    EXPECT_EQ(this->asBase(1.5).normalized(Traits::baseUnits), this->asBase(0.5));
    EXPECT_EQ(this->asBase(-1.5).normalized(Traits::baseUnits), this->asBase(-0.5));
    EXPECT_EQ(this->asBase(0.99999).normalized(Traits::baseUnits), this->asBase(0.99999));
    EXPECT_EQ(this->asBase(-0.99999).normalized(Traits::baseUnits), this->asBase(-0.99999));
    EXPECT_EQ(this->asBase(0.0001).normalized(Traits::baseUnits), this->asBase(0.0001));
    EXPECT_EQ(this->asBase(-0.0001).normalized(Traits::baseUnits), this->asBase(-0.0001));
}

TYPED_TEST(GenericMethodsTests, NormalizedPreservesSign)
{
    using Traits = TestFixture::Traits;

    for (auto unit : Traits::wellKnownUnits()) {
        std::vector positiveValues = {5.7, 10.3, 123.456};
        std::vector negativeValues = {-5.7, -10.3, -123.456};

        for (double value : positiveValues) {
            double result = TypeParam(unit, value).normalized(unit).value(unit);
            EXPECT_GT(result, 0.0);
        }

        for (double value : negativeValues) {
            double result = TypeParam(unit, value).normalized(unit).value(unit);
            EXPECT_LT(result, 0.0);
        }
    }
}

TYPED_TEST(GenericMethodsTests, NormalizedIdempotent)
{
    using Traits = TestFixture::Traits;

    for (auto targetUnit : Traits::wellKnownUnits()) {
        double testValue   = 0;
        TypeParam original = this->asBase(testValue);

        TypeParam normalizedOnce  = original.normalized(targetUnit);
        TypeParam normalizedTwice = normalizedOnce.normalized(targetUnit);

        double onceValue  = normalizedOnce.value(targetUnit);
        double twiceValue = normalizedTwice.value(targetUnit);

        EXPECT_DOUBLE_EQ(onceValue, twiceValue) << original.value(targetUnit);
    }
}

TYPED_TEST(GenericMethodsTests, NormalizedRotations)
{
    if constexpr (!std::is_same_v<TypeParam, Angle>)
        GTEST_SKIP();

    EXPECT_NEAR(Angle(AngleUnits::Degrees, 361.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                1.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Degrees, 720.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                0.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Degrees, 405.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                45.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Rotations, 5.5)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Rotations),
                0.5, this->eps);

    EXPECT_NEAR(Angle(AngleUnits::Degrees, -361.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                -1.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Degrees, -720.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                0.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Degrees, -405.0)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Degrees),
                -45.0, this->eps);
    EXPECT_NEAR(Angle(AngleUnits::Rotations, -5.5)
                    .normalized(AngleUnits::Rotations)
                    .value(AngleUnits::Rotations),
                -0.5, this->eps);
}

//=============================================================================
// Tests: value() with rounding precision
//=============================================================================
TYPED_TEST(GenericMethodsTests, ValueWithPrecisionNoRoundingNeeded)
{
    using Traits = TestFixture::Traits;

    double value          = 5.0;
    TypeParam measurement = this->asBase(value);

    for (uint8_t precision = 0; precision <= 10; ++precision) {
        double result = measurement.roundedValue(Traits::baseUnits, precision);
        EXPECT_NEAR(result, value, this->eps);
    }
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionRoundsUp)
{
    using Traits = TestFixture::Traits;

    double value          = 5.9999999;
    TypeParam measurement = this->asBase(value);

    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 0), 6.0, this->eps);
    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 1), 6.0, this->eps);
    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 6), value, 1e-6);
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionRoundsDown)
{
    using Traits = TestFixture::Traits;

    double value          = 6.0000001;
    TypeParam measurement = this->asBase(value);

    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 0), 6.0, this->eps);
    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 1), 6.0, this->eps);
    EXPECT_NEAR(measurement.roundedValue(Traits::baseUnits, 6), value, 1e-6);
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionTypicalValues)
{
    using Traits = TestFixture::Traits;

    struct TestCase
    {
        double original;
        uint8_t precision;
        double expected;
    };

    std::vector<TestCase> cases = {
        {1.0 / 3.0,   2, 0.33    }, // 0.33333... rounded to 0.33
        {1.0 / 3.0,   4, 0.3333  }, // 0.33333... rounded to 0.3333
        {2.0 / 3.0,   2, 0.67    }, // 0.66666... rounded to 0.67
        {1.23456789,  3, 1.235   }, // Round up
        {1.2344321,   3, 1.234   }, // Round down
        {123.456789,  2, 123.46  }, // Round up with larger number
        {0.000123456, 6, 0.000123}  // Very small number
    };

    for (const auto& testCase : cases) {
        TypeParam measurement = this->asBase(testCase.original);
        double result         = measurement.roundedValue(Traits::baseUnits, testCase.precision);
        EXPECT_NEAR(result, testCase.expected, std::pow(10, -testCase.precision));
    }
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionEdgeCases)
{
    using Traits = TestFixture::Traits;

    double justBelow = 5.999999999;
    double justAbove = 6.000000001;

    TypeParam below = this->asBase(justBelow);
    TypeParam above = this->asBase(justAbove);

    EXPECT_NEAR(below.roundedValue(Traits::baseUnits, 0), 6.0, this->eps);
    EXPECT_NEAR(above.roundedValue(Traits::baseUnits, 0), 6.0, this->eps);

    double resultBelow = below.roundedValue(Traits::baseUnits, 9);
    double resultAbove = above.roundedValue(Traits::baseUnits, 9);

    EXPECT_LT(resultBelow, 6.0);
    EXPECT_GT(resultAbove, 6.0);
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionNegativeNumbers)
{
    using Traits = TestFixture::Traits;
    EXPECT_NEAR(this->asBase(-5.0000001).roundedValue(Traits::baseUnits, 0), -5.0, this->eps);
    EXPECT_NEAR(this->asBase(-5.9999999).roundedValue(Traits::baseUnits, 0), -6.0, this->eps);
    EXPECT_NEAR(this->asBase(-0.0000001).roundedValue(Traits::baseUnits, 0), 0, this->eps);
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionConsistencyWithOneArg)
{
    using Traits = TestFixture::Traits;

    std::vector testValues = {0.0, 1.0, 123.456, 0.333333, 1.0 / 3.0, 2.0 / 3.0};

    for (double value : testValues) {
        TypeParam measurement = this->asBase(value);
        double oneArg         = measurement.value(Traits::baseUnits);
        double withPrecision  = measurement.roundedValue(Traits::baseUnits, 12);
        EXPECT_NEAR(oneArg, withPrecision, 1e-12);
    }
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionInDifferentUnits)
{
    using Traits = TestFixture::Traits;

    for (auto unit : Traits::wellKnownUnits()) {
        if (unit == Traits::baseUnits)
            continue;

        TypeParam measurement(unit, 1.0 / 3.0);
        double rounded = measurement.roundedValue(unit, 2);

        double epsilon = std::pow(10, -2);
        EXPECT_NEAR(rounded, 0.33, epsilon);

        TypeParam roundedMeas(unit, rounded);
        double originalValue    = measurement.value(Traits::baseUnits);
        double roundedBaseValue = roundedMeas.value(Traits::baseUnits);

        double conversionFactor = std::abs(roundedBaseValue / rounded);
        double expectedError    = epsilon * conversionFactor;

        EXPECT_NEAR(roundedBaseValue, originalValue, expectedError + 0.001);
    }
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionFloatingPointBoundary)
{
    using Traits = TestFixture::Traits;

    TypeParam a = this->asBase(15.25);
    TypeParam b = this->asBase(15.25 - 1e-13);
    TypeParam c = this->asBase(15.25 + 1e-13);

    EXPECT_TRUE(a.fuzzyEqual(b));
    EXPECT_TRUE(a.fuzzyEqual(c));

    double resultA = a.roundedValue(Traits::baseUnits, 1);
    double resultB = b.roundedValue(Traits::baseUnits, 1);
    double resultC = c.roundedValue(Traits::baseUnits, 1);

    EXPECT_EQ(resultA, resultB);
    EXPECT_EQ(resultA, resultC);
    EXPECT_TRUE(resultA == 15.2 || resultA == 15.3);
}

TYPED_TEST(GenericMethodsTests, ValueWithPrecisionDecimalRounding)
{
    using Traits = TestFixture::Traits;

    struct DecimalCase
    {
        double original;
        uint8_t precision;
        double expected;
    };

    std::vector<DecimalCase> cases = {
        {5.91691,     1, 5.9     },
        {5.91691,     2, 5.92    },
        {5.91691,     3, 5.917   },
        {1.234,       2, 1.23    },
        {1.235,       2, 1.24    },
        {1.23456789,  4, 1.2346  },
        {1.9999,      2, 2.00    },
        {1.9999,      0, 2.0     },
        {1.4999,      0, 1.0     },
        {1.5001,      0, 2.0     },
        {-5.91691,    1, -5.9    },
        {-5.91691,    2, -5.92   },
        {-1.234,      2, -1.23   },
        {-1.235,      2, -1.24   },
        {-1.236,      2, -1.24   },
        {0.000456789, 5, 0.00046 },
        {0.000444444, 5, 0.00044 },
        {12345.6789,  2, 12345.68},
        {12345.6789,  0, 12346.0 },
    };

    for (const auto& testCase : cases) {
        TypeParam measurement = this->asBase(testCase.original);
        double result         = measurement.roundedValue(Traits::baseUnits, testCase.precision);

        double epsilon = std::pow(10, -static_cast<double>(testCase.precision));
        EXPECT_NEAR(result, testCase.expected, epsilon);
    }
}
