// SPDX-License-Identifier: MIT
#include <gtest/gtest.h>
#include <measurements/Time.hpp>

class TimeTests : public ::testing::Test
{
protected:
    static constexpr double eps = 1e-10;

    Time sec(double value) { return Time(TimeUnits::Seconds, value); }

    Time min(double value) { return Time(TimeUnits::Minutes, value); }

    Time hour(double value) { return Time(TimeUnits::Hours, value); }
};

TEST_F(TimeTests, ConstructionFromStdChrono)
{
    Time secs(std::chrono::seconds(120));
    EXPECT_NEAR(secs.value(TimeUnits::Seconds), 120.0, eps);
    EXPECT_NEAR(secs.value(TimeUnits::Minutes), 2.0, eps);
    EXPECT_NEAR(secs.value(TimeUnits::Hours), 2.0 / 60.0, eps);

    Time mins(std::chrono::minutes(5));
    EXPECT_NEAR(mins.value(TimeUnits::Seconds), 300.0, eps);
    EXPECT_NEAR(mins.value(TimeUnits::Minutes), 5.0, eps);
    EXPECT_NEAR(mins.value(TimeUnits::Hours), 5.0 / 60.0, eps);

    Time hours(std::chrono::hours(3));
    EXPECT_NEAR(hours.value(TimeUnits::Seconds), 10800.0, eps);
    EXPECT_NEAR(hours.value(TimeUnits::Minutes), 180.0, eps);
    EXPECT_NEAR(hours.value(TimeUnits::Hours), 3.0, eps);

    Time msecs(std::chrono::milliseconds(1500));
    EXPECT_NEAR(msecs.value(TimeUnits::Seconds), 1.5, eps);
    EXPECT_NEAR(msecs.value(TimeUnits::Milliseconds), 1500.0, eps);

    Time usecs(std::chrono::microseconds(1234567));
    EXPECT_NEAR(usecs.value(TimeUnits::Seconds), 1.234567, eps);
    EXPECT_NEAR(usecs.value(TimeUnits::Microseconds), 1234567.0, eps);

    Time nsecs(std::chrono::nanoseconds(1234567890));
    EXPECT_NEAR(nsecs.value(TimeUnits::Seconds), 1.23456789, eps);
    EXPECT_NEAR(nsecs.value(TimeUnits::Nanoseconds), 1234567890.0, eps);
}

TEST_F(TimeTests, ConstructorFromDurationDouble)
{
    Time t(std::chrono::duration<double>(2.5));
    EXPECT_NEAR(t.value(TimeUnits::Seconds), 2.5, eps);
    EXPECT_NEAR(t.value(TimeUnits::Minutes), 2.5 / 60.0, eps);
}

TEST_F(TimeTests, ConstructorFromZeroDuration)
{
    Time t(std::chrono::seconds(0));
    EXPECT_NEAR(t.value(TimeUnits::Seconds), 0.0, eps);
    EXPECT_NEAR(t.value(TimeUnits::Minutes), 0.0, eps);
    EXPECT_NEAR(t.value(TimeUnits::Hours), 0.0, eps);
}

TEST_F(TimeTests, ConstructorFromNegativeDuration)
{
    Time t(std::chrono::seconds(-30));
    EXPECT_NEAR(t.value(TimeUnits::Seconds), -30.0, eps);
    EXPECT_NEAR(t.value(TimeUnits::Minutes), -0.5, eps);
}

TEST_F(TimeTests, EqualityWithStdChrono)
{
    Time secs = sec(120);
    EXPECT_TRUE(secs == std::chrono::seconds(120));
    EXPECT_FALSE(secs == std::chrono::seconds(121));
    EXPECT_TRUE(secs == std::chrono::minutes(2));
    EXPECT_FALSE(secs == std::chrono::minutes(3));

    Time mins = min(5);
    EXPECT_TRUE(mins == std::chrono::minutes(5));
    EXPECT_FALSE(mins == std::chrono::minutes(6));
    EXPECT_TRUE(mins == std::chrono::seconds(300));

    Time hours = hour(3);
    EXPECT_TRUE(hours == std::chrono::hours(3));
    EXPECT_FALSE(hours == std::chrono::hours(4));
    EXPECT_TRUE(hours == std::chrono::minutes(180));
    EXPECT_TRUE(hours == std::chrono::seconds(10800));

    Time msecs = sec(1.5);
    EXPECT_TRUE(msecs == std::chrono::milliseconds(1500));
    EXPECT_FALSE(msecs == std::chrono::milliseconds(1499));

    Time zero = sec(0);
    EXPECT_TRUE(zero == std::chrono::seconds(0));
    EXPECT_TRUE(zero == std::chrono::minutes(0));
    EXPECT_TRUE(zero == std::chrono::hours(0));
    EXPECT_TRUE(zero == std::chrono::milliseconds(0));

    Time negative = sec(-30);
    EXPECT_TRUE(negative == std::chrono::seconds(-30));
    EXPECT_FALSE(negative == std::chrono::seconds(30));
}

TEST_F(TimeTests, AssignmentFromStdChrono)
{
    Time secs = std::chrono::seconds(120);
    EXPECT_NEAR(secs.value(TimeUnits::Seconds), 120.0, eps);
    EXPECT_NEAR(secs.value(TimeUnits::Minutes), 2.0, eps);

    Time mins = std::chrono::minutes(5);
    EXPECT_NEAR(mins.value(TimeUnits::Seconds), 300.0, eps);
    EXPECT_NEAR(mins.value(TimeUnits::Minutes), 5.0, eps);

    Time hours = std::chrono::hours(3);
    EXPECT_NEAR(hours.value(TimeUnits::Seconds), 10800.0, eps);
    EXPECT_NEAR(hours.value(TimeUnits::Hours), 3.0, eps);
}

TEST_F(TimeTests, CastToChronoDuration)
{
    EXPECT_EQ(static_cast<std::chrono::seconds>(sec(120)).count(), 120);
    EXPECT_EQ(static_cast<std::chrono::minutes>(min(5)).count(), 5);
    EXPECT_EQ(static_cast<std::chrono::hours>(hour(3)).count(), 3);
    EXPECT_EQ(static_cast<std::chrono::milliseconds>(sec(1.5)).count(), 1500);
    EXPECT_EQ(static_cast<std::chrono::microseconds>(sec(1.234567)).count(), 1234567);
    EXPECT_EQ(static_cast<std::chrono::nanoseconds>(sec(1.2345678)).count(), 1234567800);

    EXPECT_EQ(static_cast<std::chrono::seconds>(sec(2.75)).count(), 2);
    EXPECT_EQ(static_cast<std::chrono::seconds>(-30).count(), -30);
    EXPECT_EQ(static_cast<std::chrono::seconds>(sec(0)).count(), 0);
}

TEST_F(TimeTests, RoundTripConversion)
{
    auto roundTrip = []<class T>(T duration) {
        EXPECT_EQ(duration.count(), static_cast<T>(Time(duration)).count());
    };

    roundTrip(std::chrono::seconds(123));
    roundTrip(std::chrono::minutes(45));
    roundTrip(std::chrono::hours(8));
}

TEST_F(TimeTests, CompareTimeWithDuration)
{
    Time t1 = sec(120);
    Time t2 = sec(121);

    EXPECT_TRUE(t1 == std::chrono::seconds(120));
    EXPECT_TRUE(t1 != std::chrono::seconds(121));
    EXPECT_TRUE(t1 < std::chrono::seconds(121));
    EXPECT_TRUE(t1 <= std::chrono::seconds(120));
    EXPECT_TRUE(t2 > std::chrono::seconds(120));
    EXPECT_TRUE(t2 >= std::chrono::seconds(121));
}
