#include <gtest/gtest.h>
#include <measurements/Angle.hpp>

class AngleTests : public ::testing::Test
{
protected:
    const double eps = 1e-12;

    Angle deg(double value) { return Angle(AngleUnits::Degrees, value); }

    Angle rad(double value) { return Angle(AngleUnits::Radians, value); }
};

//=============================================================================
// Sin tests
//=============================================================================

TEST_F(AngleTests, SinTests)
{
    EXPECT_NEAR(deg(0.0).sin(), 0.0, eps);
    EXPECT_NEAR(rad(0.0).sin(), 0.0, eps);

    EXPECT_NEAR(deg(-30.0).sin(), -0.5, eps);
    EXPECT_NEAR(deg(30.0).sin(), 0.5, eps);

    EXPECT_NEAR(deg(-45.0).sin(), -std::numbers::sqrt2 / 2.0, eps);
    EXPECT_NEAR(deg(45.0).sin(), std::numbers::sqrt2 / 2.0, eps);

    EXPECT_NEAR(deg(-60.0).sin(), -std::numbers::sqrt3 / 2.0, eps);
    EXPECT_NEAR(deg(60.0).sin(), std::numbers::sqrt3 / 2.0, eps);

    EXPECT_NEAR(deg(-90.0).sin(), -1.0, eps);
    EXPECT_NEAR(deg(90.0).sin(), 1.0, eps);

    EXPECT_NEAR(deg(-180.0).sin(), 0.0, eps);
    EXPECT_NEAR(deg(180.0).sin(), 0.0, eps);

    EXPECT_NEAR(deg(-270.0).sin(), 1.0, eps);
    EXPECT_NEAR(deg(270.0).sin(), -1.0, eps);

    EXPECT_NEAR(deg(-360.0).sin(), 0.0, eps);
    EXPECT_NEAR(deg(360.0).sin(), 0.0, eps);

    EXPECT_NEAR(deg(0.001).sin(), 0.001 * M_PI / 180.0, 1e-12);
}

//=============================================================================
// Cos tests
//=============================================================================

TEST_F(AngleTests, CosTests)
{
    EXPECT_NEAR(deg(0.0).cos(), 1.0, eps);
    EXPECT_NEAR(rad(0.0).cos(), 1.0, eps);

    EXPECT_NEAR(deg(-30.0).cos(), std::numbers::sqrt3 / 2.0, eps);
    EXPECT_NEAR(deg(30.0).cos(), std::numbers::sqrt3 / 2.0, eps);

    EXPECT_NEAR(deg(-45.0).cos(), std::numbers::sqrt2 / 2.0, eps);
    EXPECT_NEAR(deg(45.0).cos(), std::numbers::sqrt2 / 2.0, eps);

    EXPECT_NEAR(deg(-60.0).cos(), 0.5, eps);
    EXPECT_NEAR(deg(60.0).cos(), 0.5, eps);

    EXPECT_NEAR(deg(-90.0).cos(), 0.0, eps);
    EXPECT_NEAR(deg(90.0).cos(), 0.0, eps);

    EXPECT_NEAR(deg(-180.0).cos(), -1.0, eps);
    EXPECT_NEAR(deg(180.0).cos(), -1.0, eps);

    EXPECT_NEAR(deg(-270.0).cos(), 0.0, eps);
    EXPECT_NEAR(deg(270.0).cos(), 0.0, eps);

    EXPECT_NEAR(deg(-360.0).cos(), 1.0, eps);
    EXPECT_NEAR(deg(360.0).cos(), 1.0, eps);

    EXPECT_NEAR(deg(390.0).cos(), std::numbers::sqrt3 / 2.0, eps);
    EXPECT_NEAR(deg(450.0).cos(), 0.0, eps);
    EXPECT_NEAR(deg(540.0).cos(), -1.0, eps);
    EXPECT_NEAR(deg(720.0).cos(), 1.0, eps);
}

//=============================================================================
// Tan tests
//=============================================================================

TEST_F(AngleTests, TanTests)
{
    EXPECT_NEAR(deg(0.0).tan(), 0.0, eps);
    EXPECT_NEAR(rad(0.0).tan(), 0.0, eps);

    EXPECT_NEAR(deg(-30.0).tan(), -1.0 / std::numbers::sqrt3, eps);
    EXPECT_NEAR(deg(30.0).tan(), 1.0 / std::numbers::sqrt3, eps);

    EXPECT_NEAR(deg(-45.0).tan(), -1.0, eps);
    EXPECT_NEAR(deg(45.0).tan(), 1.0, eps);

    EXPECT_NEAR(deg(-60.0).tan(), -std::numbers::sqrt3, eps);
    EXPECT_NEAR(deg(60.0).tan(), std::numbers::sqrt3, eps);

    EXPECT_NEAR(deg(90).sin(), 1.0, 1e-15);
    EXPECT_NEAR(deg(-90).sin(), -1.0, 1e-15);
    EXPECT_NEAR(deg(90).cos(), 0.0, 1e-15);
    EXPECT_NEAR(deg(-90).cos(), 0.0, 1e-15);

    EXPECT_GT(std::abs(deg(90).tan()), 1e15);
    EXPECT_GT(std::abs(deg(-90).tan()), 1e15);

    EXPECT_NEAR(deg(90).cot(), 0.0, 1e-15);
    EXPECT_NEAR(deg(-90).cot(), 0.0, 1e-15);
    EXPECT_NEAR(deg(90).tan() / deg(-90).tan(), -1.0, 1e-15);
    EXPECT_NEAR(deg(90).tan() * deg(90).cot(), 1.0, 1e-15);

    EXPECT_NEAR(deg(-180.0).tan(), 0.0, eps);
    EXPECT_NEAR(deg(180.0).tan(), 0.0, eps);

    EXPECT_NEAR(deg(-360.0).tan(), 0.0, eps);
    EXPECT_NEAR(deg(360.0).tan(), 0.0, eps);

    EXPECT_NEAR(deg(390.0).tan(), 1.0 / std::numbers::sqrt3, eps);
    EXPECT_NEAR(deg(540.0).tan(), 0.0, eps);
    EXPECT_NEAR(deg(720.0).tan(), 0.0, eps);
}

//=============================================================================
// Trigonometric identities
//=============================================================================

TEST_F(AngleTests, SinSquaredPlusCosSquared)
{
    std::vector angles = {0.0, 30.0, 45.0, 60.0, 90.0, 120.0, 180.0, 270.0, 360.0};

    for (double degAngle : angles) {
        Angle angle = deg(degAngle);
        double s    = angle.sin();
        double c    = angle.cos();
        EXPECT_NEAR(s * s + c * c, 1.0, eps);
    }
}

TEST_F(AngleTests, TanEqualsSinOverCos)
{
    std::vector angles = {0.0, 30.0, 45.0, 60.0, 180.0, 360.0};

    for (double degAngle : angles) {
        Angle angle       = deg(degAngle);
        double tanVal     = angle.tan();
        double sinOverCos = angle.sin() / angle.cos();
        EXPECT_NEAR(tanVal, sinOverCos, eps);
    }
}

//=============================================================================
// signedArc
//=============================================================================
TEST_F(AngleTests, signedArcSameAngle)
{
    EXPECT_NEAR(deg(0.0).signedArc(deg(0.0)).value(AngleUnits::Degrees), 0.0, eps);
    EXPECT_NEAR(deg(45.0).signedArc(deg(45.0)).value(AngleUnits::Degrees), 0.0, eps);
    EXPECT_NEAR(deg(180.0).signedArc(deg(180.0)).value(AngleUnits::Degrees), 0.0, eps);
    EXPECT_NEAR(deg(270.0).signedArc(deg(270.0)).value(AngleUnits::Degrees), 0.0, eps);
    EXPECT_NEAR(deg(360.0).signedArc(deg(360.0)).value(AngleUnits::Degrees), 0.0, eps);
}

TEST_F(AngleTests, signedArcNearbyAngles)
{
    EXPECT_NEAR(deg(0.0).signedArc(deg(5.0)).value(AngleUnits::Degrees), 5.0, eps);
    EXPECT_NEAR(deg(10.0).signedArc(deg(15.0)).value(AngleUnits::Degrees), 5.0, eps);
    EXPECT_NEAR(deg(350.0).signedArc(deg(355.0)).value(AngleUnits::Degrees), 5.0, eps);
    EXPECT_NEAR(deg(355.0).signedArc(deg(360.0)).value(AngleUnits::Degrees), 5.0, eps);
    EXPECT_NEAR(deg(355.0).signedArc(deg(0.0)).value(AngleUnits::Degrees), 5.0, eps);

    EXPECT_NEAR(deg(5.0).signedArc(deg(0.0)).value(AngleUnits::Degrees), -5.0, eps);
    EXPECT_NEAR(deg(15.0).signedArc(deg(10.0)).value(AngleUnits::Degrees), -5.0, eps);
    EXPECT_NEAR(deg(355.0).signedArc(deg(350.0)).value(AngleUnits::Degrees), -5.0, eps);
    EXPECT_NEAR(deg(0.0).signedArc(deg(355.0)).value(AngleUnits::Degrees), -5.0, eps);
    EXPECT_NEAR(deg(5.0).signedArc(deg(360.0)).value(AngleUnits::Degrees), -5.0, eps);
}

TEST_F(AngleTests, DistanceAcrossZero)
{
    EXPECT_NEAR(deg(358.0).signedArc(deg(3.0)).value(AngleUnits::Degrees), 5.0, eps);
    EXPECT_NEAR(deg(3.0).signedArc(deg(358.0)).value(AngleUnits::Degrees), -5.0, eps);
    EXPECT_NEAR(deg(359.0).signedArc(deg(1.0)).value(AngleUnits::Degrees), 2.0, eps);
    EXPECT_NEAR(deg(1.0).signedArc(deg(359.0)).value(AngleUnits::Degrees), -2.0, eps);
    EXPECT_NEAR(deg(350.0).signedArc(deg(10.0)).value(AngleUnits::Degrees), 20.0, eps);
    EXPECT_NEAR(deg(10.0).signedArc(deg(350.0)).value(AngleUnits::Degrees), -20.0, eps);
}

TEST_F(AngleTests, DistanceExactlyHalfCircle)
{
    EXPECT_NEAR(deg(0.0).signedArc(deg(180.0)).value(AngleUnits::Degrees), 180.0, eps);
    EXPECT_NEAR(deg(180.0).signedArc(deg(0.0)).value(AngleUnits::Degrees), -180.0, eps);
    EXPECT_NEAR(deg(90.0).signedArc(deg(270.0)).value(AngleUnits::Degrees), 180.0, eps);
    EXPECT_NEAR(deg(270.0).signedArc(deg(90.0)).value(AngleUnits::Degrees), -180.0, eps);
    EXPECT_NEAR(deg(45.0).signedArc(deg(225.0)).value(AngleUnits::Degrees), 180.0, eps);
    EXPECT_NEAR(deg(225.0).signedArc(deg(45.0)).value(AngleUnits::Degrees), -180.0, eps);
}

TEST_F(AngleTests, DistanceNeverExceeds180)
{
    for (int a = 0; a < 360; a += 15) {
        for (int b = 0; b < 360; b += 15) {
            double dist = deg(a).signedArc(deg(b)).value(AngleUnits::Degrees);
            EXPECT_LE(std::abs(dist), 180.0 + eps);
        }
    }
}

TEST_F(AngleTests, DistanceSignIndicatesDirection)
{
    EXPECT_LT(deg(10.0).signedArc(deg(5.0)).value(AngleUnits::Degrees), 0.0);
    EXPECT_GT(deg(5.0).signedArc(deg(10.0)).value(AngleUnits::Degrees), 0.0);
    EXPECT_NEAR(deg(10.0).signedArc(deg(280.0)).value(AngleUnits::Degrees), -90.0, eps);
    EXPECT_NEAR(deg(280.0).signedArc(deg(10.0)).value(AngleUnits::Degrees), 90.0, eps);
}

TEST_F(AngleTests, DistanceIsAntiSymmetric)
{
    std::vector angles = {0.0, 30.0, 45.0, 90.0, 180.0, 270.0, 300.0, 358.0, 359.0, 360.0};

    for (double a : angles) {
        for (double b : angles) {
            double distAB = deg(a).signedArc(deg(b)).value(AngleUnits::Degrees);
            double distBA = deg(b).signedArc(deg(a)).value(AngleUnits::Degrees);
            EXPECT_NEAR(distAB, -distBA, eps);
        }
    }
}

TEST_F(AngleTests, DistanceWithAnglesBeyond360)
{
    EXPECT_NEAR(deg(361.0).signedArc(deg(5.0)).value(AngleUnits::Degrees), 4.0, eps);
    EXPECT_NEAR(deg(5.0).signedArc(deg(361.0)).value(AngleUnits::Degrees), -4.0, eps);
    EXPECT_NEAR(deg(358.0).signedArc(deg(365.0)).value(AngleUnits::Degrees), 7.0, eps);
    EXPECT_NEAR(deg(365.0).signedArc(deg(358.0)).value(AngleUnits::Degrees), -7.0, eps);
    EXPECT_NEAR(deg(720.0).signedArc(deg(90.0)).value(AngleUnits::Degrees), 90.0, eps);
    EXPECT_NEAR(deg(90.0).signedArc(deg(720.0)).value(AngleUnits::Degrees), -90.0, eps);
}

TEST_F(AngleTests, DistanceWithNegativeAngles)
{
    EXPECT_NEAR(deg(-1.0).signedArc(deg(5.0)).value(AngleUnits::Degrees), 6.0, eps);
    EXPECT_NEAR(deg(5.0).signedArc(deg(-1.0)).value(AngleUnits::Degrees), -6.0, eps);
    EXPECT_NEAR(deg(-5.0).signedArc(deg(358.0)).value(AngleUnits::Degrees), 3.0, eps);
    EXPECT_NEAR(deg(358.0).signedArc(deg(-5.0)).value(AngleUnits::Degrees), -3.0, eps);
    EXPECT_NEAR(deg(-90.0).signedArc(deg(90.0)).value(AngleUnits::Degrees), 180.0, eps);
    EXPECT_NEAR(deg(-180.0).signedArc(deg(180.0)).value(AngleUnits::Degrees), 0.0, eps);
}
