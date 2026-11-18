// SPDX-License-Identifier: MIT
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>

#include <gtest/gtest.h>
#include <measurements/prototypes/Measurement.hpp>

// ----------------------------------------------------------------------
// Helper: Verify that the result is an exact multiple of 10^-precision
// (within floating‑point tolerance) and that the absolute error is
// bounded by one full unit in the last place (plus a tiny epsilon).
// ----------------------------------------------------------------------
namespace {
void expectValidRounding(double input, uint8_t precision)
{
    const double result = roundedDouble(input, precision);
    const double scale  = std::pow(10.0, precision);
    const double scaled = result * scale;

    // Check that scaled result is essentially an integer
    double intPart    = 0;
    const double frac = std::modf(scaled, &intPart);
    EXPECT_NEAR(frac, 0.0, 1e-12 * std::max(1.0, std::abs(scaled)))
        << "Result is not a multiple of 10^-" << static_cast<int>(precision) << " for input "
        << input;

    // The error must not exceed one full step (allowing for the nudge)
    const double step  = std::pow(10.0, -precision);
    const double error = std::abs(input - result);
    EXPECT_LE(error, step + 1e-12 * std::max(1.0, std::abs(input)))
        << "Rounding error too large for input " << input << " at precision "
        << static_cast<int>(precision);
}
} // unnamed namespace

// ======================================================================
// 1. Basic rounding (no nudge applied)
// ======================================================================
TEST(RoundingTests, BasicRoundingPrecision0)
{
    EXPECT_DOUBLE_EQ(roundedDouble(3.2, 0), 3.0);
    EXPECT_DOUBLE_EQ(roundedDouble(3.7, 0), 4.0);
    EXPECT_DOUBLE_EQ(roundedDouble(-2.3, 0), -2.0);
    EXPECT_DOUBLE_EQ(roundedDouble(-2.8, 0), -3.0);
}

TEST(RoundingTests, BasicRoundingPrecision2)
{
    EXPECT_DOUBLE_EQ(roundedDouble(1.2345, 2), 1.23);
    EXPECT_DOUBLE_EQ(roundedDouble(1.235, 2), 1.24); // round half away from zero
    EXPECT_DOUBLE_EQ(roundedDouble(-1.235, 2), -1.24);
}

// ======================================================================
// 2. The nudge mechanism: values extremely close to a half‑integer
// ======================================================================
TEST(RoundingTests, NudgeAppliedPositiveTinyBelowHalf)
{
    // 1.5 - 1e-13  → distance to 0.5 is 1e-13 (< tolerance ~1.5e-12)
    // Without nudge it would round to 1.0; with nudge it rounds to 2.0.
    EXPECT_DOUBLE_EQ(roundedDouble(1.5 - 1e-13, 0), 2.0);
}

TEST(RoundingTests, NudgeAppliedNegativeTinyAboveHalf)
{
    // -1.5 + 1e-13 → fractional part 0.4999999999999, scaled tolerance
    // pushes it toward -2.0.
    EXPECT_DOUBLE_EQ(roundedDouble(-1.5 + 1e-13, 0), -2.0);
}

TEST(RoundingTests, NoNudgeWhenDistTooLarge)
{
    // 1.5 - 1e-11 → dist = 1e-11 > tolerance (~1.5e-12) → no nudge
    EXPECT_DOUBLE_EQ(roundedDouble(1.5 - 1e-11, 0), 1.0);
    EXPECT_DOUBLE_EQ(roundedDouble(-1.5 + 1e-11, 0), -1.0);
}

// ======================================================================
// 3. The core contract: fuzzy‑equal inputs must round identically
// ======================================================================
TEST(RoundingTests, FuzzyEqualPairsRoundToSame)
{
    constexpr double eps = 1e-13; // well inside qFuzzyCompare tolerance

    // Pair around a non‑half value → both round the same way anyway
    double a = 2.3;
    double b = a + eps;
    EXPECT_DOUBLE_EQ(roundedDouble(a, 1), roundedDouble(b, 1));

    // Pair straddling a half‑integer → nudge aligns them
    double c = 4.5 - 5e-13; // just below 4.5
    double d = 4.5 + 3e-13; // just above 4.5
    // Both are within tolerance of the half → both should round to 5.0
    EXPECT_DOUBLE_EQ(roundedDouble(c, 0), 5.0);
    EXPECT_DOUBLE_EQ(roundedDouble(d, 0), 5.0);
    EXPECT_DOUBLE_EQ(roundedDouble(c, 0), roundedDouble(d, 0));
}

// ======================================================================
// 4. Large values where tolerance >= 0.5 → nudge is skipped,
//    but rounding still works (no fallback to the original input).
// ======================================================================
TEST(RoundingTests, LargeValuesNoNudge)
{
    // For 1e13, scaled tolerance = 1e-12 * 1e13 = 10 >= 0.5
    double val1 = 1e13 + 0.3;
    double val2 = 1e13 - 0.3;
    EXPECT_DOUBLE_EQ(roundedDouble(val1, 0), 1e13);
    EXPECT_DOUBLE_EQ(roundedDouble(val2, 0), 1e13);

    // With a precision > 0 the nudge is still skipped for huge numbers.
    double val3 = 5e15 + 12345.6789;
    EXPECT_DOUBLE_EQ(roundedDouble(val3, 3), std::round(val3 * 1e3) / 1e3);
}

// ======================================================================
// 5. Special values: zero, negative zero, NaN, infinities
// ======================================================================
TEST(RoundingTests, ZeroAndNegativeZero)
{
    double positiveZero = 0.0;
    double negativeZero = -0.0;
    // Both should round to themselves (keeping sign for negative zero)
    EXPECT_DOUBLE_EQ(roundedDouble(positiveZero, 3), 0.0);
    EXPECT_TRUE(std::signbit(roundedDouble(negativeZero, 3)));
}

TEST(RoundingTests, NaNandInf)
{
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    EXPECT_TRUE(std::isnan(roundedDouble(nan, 0)));
    EXPECT_TRUE(std::isinf(roundedDouble(inf, 0)));
    EXPECT_EQ(roundedDouble(inf, 0), inf);
    EXPECT_EQ(roundedDouble(-inf, 0), -inf);
}

// ======================================================================
// 6. High precision (up to the representability limit)
// ======================================================================
TEST(RoundingTests, HighPrecision)
{
    // 3.141592653589793 → at precision 15 we expect 3.141592653589793
    // (double itself may not hold all digits exactly; use a tolerance)
    double roundedPI = roundedDouble(std::numbers::pi, 15);
    // The result must be within ~0.5e-15 of the exact rounding
    EXPECT_NEAR(roundedPI, 3.141592653589793, 1e-15);
    expectValidRounding(std::numbers::pi, 15);
}

// ======================================================================
// 7. Property‑based sanity check with a variety of values & precisions
// ======================================================================
TEST(RoundingTests, PropertyValidRoundingManyValues)
{
    constexpr std::array testValues{
        0.0,  1.0,   -1.0,        123.456,      -9876.54321,          1e-10,
        1e10, 1e-12, 0.000000001, -0.000000001, 1.234567890123456e15, -1e-15};
    for (double v : testValues) {
        for (uint8_t prec : {0, 1, 2, 5, 10, 15}) {
            expectValidRounding(v, prec);
        }
    }
}
