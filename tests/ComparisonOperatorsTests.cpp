// SPDX-License-Identifier: MIT
#include "GenericTestSuite.hpp"

template <class T>
class ComparisonOperatorsTests : public GenericTestSuite<T>
{
};

TYPED_TEST_SUITE(ComparisonOperatorsTests, Types);

//=============================================================================
// Equality operators (== and !=)
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, EqualityWithSelf)
{
    EXPECT_EQ(this->asBase(1), this->asBase(1));
    EXPECT_EQ(this->asBase(0), this->asBase(0));
    EXPECT_EQ(this->asBase(-1), this->asBase(-1));
    EXPECT_EQ(this->asBase(10), this->asBase(10));
    EXPECT_EQ(this->asBase(100), this->asBase(100));
}

TYPED_TEST(ComparisonOperatorsTests, EqualityWithSameValue)
{
    EXPECT_EQ(this->asBase(42), this->asBase(42));
    EXPECT_EQ(this->asBase(1.0000000001), this->asBase(1.0000000001));
    EXPECT_EQ(this->asBase(2), TypeParam(this->asBase(2)));
}

TYPED_TEST(ComparisonOperatorsTests, EqualityWithZero)
{
    EXPECT_EQ(this->asBase(0), this->asBase(0));
    EXPECT_NE(this->asBase(0.00000001), this->asBase(0));
    EXPECT_NE(this->asBase(-0.00000001), this->asBase(0));
}

TYPED_TEST(ComparisonOperatorsTests, InequalityOfDifferentValues)
{
    EXPECT_NE(this->asBase(1), this->asBase(2));
    EXPECT_NE(this->asBase(1), this->asBase(-1));
    EXPECT_NE(this->asBase(0), this->asBase(1));
    EXPECT_NE(this->asBase(10), this->asBase(100));
    EXPECT_NE(this->asBase(-1), this->asBase(-2));
    EXPECT_NE(this->asBase(-1), this->asBase(1));
    EXPECT_NE(this->asBase(42), this->asBase(43));
    EXPECT_NE(this->asBase(1.0000000001), this->asBase(1.0000000002));
}

TYPED_TEST(ComparisonOperatorsTests, EqualityIsSymmetric)
{
    EXPECT_EQ(this->asBase(1), this->asBase(1));
    EXPECT_EQ(this->asBase(1), this->asBase(1));

    EXPECT_NE(this->asBase(1), this->asBase(2));
    EXPECT_NE(this->asBase(2), this->asBase(1));

    EXPECT_EQ(this->asBase(42), this->asBase(42));
    EXPECT_EQ(this->asBase(42), this->asBase(42));
}

TYPED_TEST(ComparisonOperatorsTests, EqualityIsTransitive)
{
    TypeParam x = this->asBase(5);
    TypeParam y = this->asBase(5);
    TypeParam z = this->asBase(5);

    ASSERT_EQ(x, y);
    ASSERT_EQ(y, z);
    EXPECT_EQ(x, z);
}

TYPED_TEST(ComparisonOperatorsTests, EqualityIsReflexive)
{
    std::vector<TypeParam> values = {this->asBase(0),   this->asBase(1),    this->asBase(2),
                                     this->asBase(-1),  this->asBase(10),   this->asBase(100),
                                     this->asBase(-10), this->asBase(1e10), this->asBase(1e-10)};

    for (const auto& asBase : values) {
        EXPECT_EQ(asBase, asBase);
    }
}

TYPED_TEST(ComparisonOperatorsTests, InequalityOperatorOppositeOfEquality)
{
    EXPECT_EQ(this->asBase(1) == this->asBase(1), !(this->asBase(1) != this->asBase(1)));
    EXPECT_EQ(this->asBase(1) == this->asBase(2), !(this->asBase(1) != this->asBase(2)));
    EXPECT_EQ(this->asBase(42) == this->asBase(42), !(this->asBase(42) != this->asBase(42)));
    EXPECT_EQ(this->asBase(42) == this->asBase(43), !(this->asBase(42) != this->asBase(43)));
    EXPECT_EQ(this->asBase(0) == this->asBase(0.00000001),
              !(this->asBase(0) != this->asBase(0.00000001)));
}

//=============================================================================
// Less than operator (<)
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, LessThanWithSelf)
{
    // TRIVIAL CASE: a < a should be false
    EXPECT_FALSE(this->asBase(1) < this->asBase(1));
    EXPECT_FALSE(this->asBase(0) < this->asBase(0));
    EXPECT_FALSE(this->asBase(-1) < this->asBase(-1));
    EXPECT_FALSE(this->asBase(10) < this->asBase(10));
    EXPECT_FALSE(this->asBase(100) < this->asBase(100));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanWithPositiveNumbers)
{
    EXPECT_TRUE(this->asBase(1) < this->asBase(2));
    EXPECT_TRUE(this->asBase(2) < this->asBase(3));
    EXPECT_TRUE(this->asBase(2) < this->asBase(5));
    EXPECT_TRUE(this->asBase(1) < this->asBase(10));
    EXPECT_TRUE(this->asBase(10) < this->asBase(100));
    EXPECT_TRUE(this->asBase(1e-10) < this->asBase(1e10));

    EXPECT_FALSE(this->asBase(2) < this->asBase(1));
    EXPECT_FALSE(this->asBase(3) < this->asBase(2));
    EXPECT_FALSE(this->asBase(10) < this->asBase(1));
    EXPECT_FALSE(this->asBase(1e10) < this->asBase(1e-10));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanWithNegativeNumbers)
{
    EXPECT_TRUE(this->asBase(-2) < this->asBase(-1));
    EXPECT_TRUE(this->asBase(-3) < this->asBase(-1));
    EXPECT_TRUE(this->asBase(-10) < this->asBase(-1));
    EXPECT_TRUE(this->asBase(-10) < this->asBase(-2));
    EXPECT_TRUE(this->asBase(-1e10) < this->asBase(-1e-10));

    EXPECT_FALSE(this->asBase(-1) < this->asBase(-2));
    EXPECT_FALSE(this->asBase(-1) < this->asBase(-3));
    EXPECT_FALSE(this->asBase(-1e-10) < this->asBase(-1e10));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanWithMixedSigns)
{
    EXPECT_TRUE(this->asBase(-1) < this->asBase(0));
    EXPECT_TRUE(this->asBase(-10) < this->asBase(0));
    EXPECT_TRUE(this->asBase(-1) < this->asBase(1));
    EXPECT_TRUE(this->asBase(0) < this->asBase(1));
    EXPECT_TRUE(this->asBase(-5) < this->asBase(5));

    EXPECT_FALSE(this->asBase(0) < this->asBase(-1));
    EXPECT_FALSE(this->asBase(1) < this->asBase(-1));
    EXPECT_FALSE(this->asBase(1) < this->asBase(0));
    EXPECT_FALSE(this->asBase(5) < this->asBase(-5));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanWithZero)
{
    EXPECT_FALSE(this->asBase(0) < this->asBase(0));
    EXPECT_FALSE(this->asBase(0.00000001) < this->asBase(0));
    EXPECT_TRUE(this->asBase(0) < this->asBase(0.00000001));
    EXPECT_TRUE(this->asBase(-0.00000001) < this->asBase(0));
    EXPECT_FALSE(this->asBase(0) < this->asBase(-0.00000001));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanIsTransitive)
{
    ASSERT_TRUE(this->asBase(2.5) < this->asBase(5.0));
    ASSERT_TRUE(this->asBase(5.0) < this->asBase(7.5));
    EXPECT_TRUE(this->asBase(2.5) < this->asBase(7.5));

    ASSERT_TRUE(this->asBase(-7.5) < this->asBase(-5.0));
    ASSERT_TRUE(this->asBase(-5.0) < this->asBase(-2.5));
    EXPECT_TRUE(this->asBase(-7.5) < this->asBase(-2.5));

    ASSERT_TRUE(this->asBase(-0.00000001) < this->asBase(0));
    ASSERT_TRUE(this->asBase(0) < this->asBase(0.00000001));
    EXPECT_TRUE(this->asBase(-0.00000001) < this->asBase(0.00000001));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanIsAntiSymmetric)
{
    ASSERT_TRUE(this->asBase(1) < this->asBase(2));
    EXPECT_FALSE(this->asBase(2) < this->asBase(1));

    ASSERT_TRUE(this->asBase(-2) < this->asBase(-1));
    EXPECT_FALSE(this->asBase(-1) < this->asBase(-2));

    ASSERT_TRUE(this->asBase(0) < this->asBase(1));
    EXPECT_FALSE(this->asBase(1) < this->asBase(0));

    ASSERT_TRUE(this->asBase(-1) < this->asBase(0));
    EXPECT_FALSE(this->asBase(0) < this->asBase(-1));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanConsistentWithEquality)
{
    auto testCases = std::vector<std::tuple<double, double, int>>{
        {1.0,          1.0,          0 }, // equal
        {1.0,          2.0,          -1}, // a < b
        {2.0,          1.0,          1 }, // b < a
        {0.0,          -1.0,         1 }, // b < a
        {-5.0,         -3.0,         -1}, // a < b
        {-2.0,         -5.0,         1 }, // b < a
        {0.0,          0.0,          0 }, // equal
        {1.0000000001, 1.0000000002, -1}, // a < b (very close)
        {1.0000000002, 1.0000000001, 1 }  // b < a
    };

    for (const auto& [v1, v2, expected] : testCases) {
        auto a = this->asBase(v1);
        auto b = this->asBase(v2);

        bool less    = a < b;
        bool equal   = a == b;
        bool greater = b < a;

        if (expected < 0) {
            EXPECT_TRUE(less);
            EXPECT_FALSE(equal);
            EXPECT_FALSE(greater);
        } else if (expected > 0) {
            EXPECT_FALSE(less);
            EXPECT_FALSE(equal);
            EXPECT_TRUE(greater);
        } else {
            EXPECT_FALSE(less);
            EXPECT_TRUE(equal);
            EXPECT_FALSE(greater);
        }

        int trueCount = (less ? 1 : 0) + (equal ? 1 : 0) + (greater ? 1 : 0);
        EXPECT_EQ(trueCount, 1);
    }
}

//=============================================================================
// Less than or equal operator (<=)
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, LessThanOrEqualWithSelf)
{
    EXPECT_TRUE(this->asBase(1) <= this->asBase(1));
    EXPECT_TRUE(this->asBase(0) <= this->asBase(0));
    EXPECT_TRUE(this->asBase(-1) <= this->asBase(-1));
    EXPECT_TRUE(this->asBase(10) <= this->asBase(10));
    EXPECT_TRUE(this->asBase(100) <= this->asBase(100));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanOrEqualWithDifferentValues)
{
    EXPECT_TRUE(this->asBase(1) <= this->asBase(2));
    EXPECT_TRUE(this->asBase(-2) <= this->asBase(-1));
    EXPECT_TRUE(this->asBase(0) <= this->asBase(1));
    EXPECT_TRUE(this->asBase(-1) <= this->asBase(0));
    EXPECT_TRUE(this->asBase(1e-10) <= this->asBase(1e10));
    EXPECT_TRUE(this->asBase(-1e10) <= this->asBase(-1e-10));

    EXPECT_FALSE(this->asBase(2) <= this->asBase(1));
    EXPECT_FALSE(this->asBase(1) <= this->asBase(0));
    EXPECT_FALSE(this->asBase(-1) <= this->asBase(-2));
    EXPECT_FALSE(this->asBase(1e10) <= this->asBase(1e-10));
    EXPECT_FALSE(this->asBase(-1e-10) <= this->asBase(-1e10));
}

TYPED_TEST(ComparisonOperatorsTests, LessThanOrEqualConsistency)
{
    // a <= b should be equivalent to (a < b) || (a == b)
    std::vector<std::pair<double, double>> pairs = {
        {1.0,          2.0         }, // less
        {2.0,          1.0         }, // greater
        {3.0,          3.0         }, // equal
        {0.0,          -1.0        }, // greater (negative)
        {-5.0,         -3.0        }, // less (negative)
        {-2.0,         -2.0        }, // equal (negative)
        {1.0000000001, 1.0000000002}, // less (very close)
        {1.0000000002, 1.0000000001}, // greater (very close)
        {1.0000000001, 1.0000000001}  // equal (very close)
    };

    for (const auto& [v1, v2] : pairs) {
        auto a = this->asBase(v1);
        auto b = this->asBase(v2);
        EXPECT_EQ(a <= b, a < b || a == b);
    }
}

//=============================================================================
// Greater than operator (>)
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, GreaterThanWithSelf)
{
    EXPECT_FALSE(this->asBase(1) > this->asBase(1));
    EXPECT_FALSE(this->asBase(0) > this->asBase(0));
    EXPECT_FALSE(this->asBase(-1) > this->asBase(-1));
    EXPECT_FALSE(this->asBase(10) > this->asBase(10));
}

TYPED_TEST(ComparisonOperatorsTests, GreaterThanConsistencyWithLessThan)
{
    std::vector<std::pair<double, double>> testPairs = {
        {5.0,          3.0         }, // a > b
        {2.0,          7.0         }, // a < b
        {4.0,          4.0         }, // equal
        {-1.0,         -5.0        }, // a > b
        {-10.0,        -2.0        }, // a < b
        {0.0,          -1.0        }, // a > b
        {1.0000000002, 1.0000000001}, // a > b
        {1.0000000001, 1.0000000002}  // a < b
    };

    for (const auto& [v1, v2] : testPairs) {
        auto a = this->asBase(v1);
        auto b = this->asBase(v2);

        EXPECT_EQ(a > b, b < a);
    }
}

TYPED_TEST(ComparisonOperatorsTests, GreaterThanWithNumbers)
{
    EXPECT_TRUE(this->asBase(2) > this->asBase(1));
    EXPECT_TRUE(this->asBase(10) > this->asBase(1));
    EXPECT_TRUE(this->asBase(0) > this->asBase(-1));
    EXPECT_TRUE(this->asBase(-1) > this->asBase(-2));
    EXPECT_TRUE(this->asBase(1e10) > this->asBase(1e-10));
    EXPECT_TRUE(this->asBase(-1e-10) > this->asBase(-1e10));

    EXPECT_FALSE(this->asBase(1) > this->asBase(2));
    EXPECT_FALSE(this->asBase(-2) > this->asBase(-1));
    EXPECT_FALSE(this->asBase(1e-10) > this->asBase(1e10));
    EXPECT_FALSE(this->asBase(-1e10) > this->asBase(-1e-10));
}

//=============================================================================
// Greater than or equal operator (>=)
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, GreaterThanOrEqualWithSelf)
{
    EXPECT_TRUE(this->asBase(1) >= this->asBase(1));
    EXPECT_TRUE(this->asBase(0) >= this->asBase(0));
    EXPECT_TRUE(this->asBase(-1) >= this->asBase(-1));
    EXPECT_TRUE(this->asBase(10) >= this->asBase(10));
    EXPECT_TRUE(this->asBase(100) >= this->asBase(100));
}

TYPED_TEST(ComparisonOperatorsTests, GreaterThanOrEqualWithDifferentValues)
{
    EXPECT_TRUE(this->asBase(2) >= this->asBase(1));
    EXPECT_TRUE(this->asBase(-1) >= this->asBase(-2));
    EXPECT_TRUE(this->asBase(1) >= this->asBase(0));
    EXPECT_TRUE(this->asBase(0) >= this->asBase(-1));
    EXPECT_TRUE(this->asBase(1e10) >= this->asBase(1e-10));
    EXPECT_TRUE(this->asBase(-1e-10) >= this->asBase(-1e10));

    EXPECT_FALSE(this->asBase(1) >= this->asBase(2));
    EXPECT_FALSE(this->asBase(0) >= this->asBase(1));
    EXPECT_FALSE(this->asBase(-2) >= this->asBase(-1));
    EXPECT_FALSE(this->asBase(1e-10) >= this->asBase(1e10));
    EXPECT_FALSE(this->asBase(-1e10) >= this->asBase(-1e-10));
}

TYPED_TEST(ComparisonOperatorsTests, GreaterThanOrEqualConsistency)
{
    std::vector<std::pair<double, double>> testPairs = {
        {5.0,          3.0         }, // greater
        {2.0,          7.0         }, // less
        {3.0,          3.0         }, // equal
        {-1.0,         -5.0        }, // greater
        {-10.0,        -2.0        }, // less
        {0.0,          -1.0        }, // greater
        {1.0000000002, 1.0000000001}, // greater
        {1.0000000001, 1.0000000002}, // less
        {1.0000000001, 1.0000000001}  // equal
    };

    for (const auto& [v1, v2] : testPairs) {
        auto a = this->asBase(v1);
        auto b = this->asBase(v2);
        EXPECT_EQ(a >= b, a > b || a == b);
    }
}

//=============================================================================
// Cross-operator consistency tests
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, AllOperatorsConsistent)
{
    std::vector<double> testValues = {-1e10, -10, -5, -1, -0.00000001, 0,   0.00000001,
                                      1,     2,   5,  10, 100,         1e10};

    for (size_t i = 0; i < testValues.size(); ++i) {
        for (size_t j = 0; j < testValues.size(); ++j) {
            auto a = this->asBase(testValues[i]);
            auto b = this->asBase(testValues[j]);

            if (i < j) {
                EXPECT_LT(a, b);
                EXPECT_LE(a, b);
                EXPECT_GT(b, a);
                EXPECT_GE(b, a);
                EXPECT_NE(a, b);
                EXPECT_FALSE(a == b);
                EXPECT_FALSE(a > b);
                EXPECT_FALSE(a >= b);
                EXPECT_FALSE(b < a);
                EXPECT_FALSE(b <= a);
            } else if (i > j) {
                EXPECT_GT(a, b);
                EXPECT_GE(a, b);
                EXPECT_LT(b, a);
                EXPECT_LE(b, a);
                EXPECT_NE(a, b);
                EXPECT_FALSE(a == b);
                EXPECT_FALSE(a < b);
                EXPECT_FALSE(a <= b);
                EXPECT_FALSE(b > a);
                EXPECT_FALSE(b >= a);
            } else {
                EXPECT_EQ(a, b);
                EXPECT_LE(a, b);
                EXPECT_GE(a, b);
                EXPECT_FALSE(a < b);
                EXPECT_FALSE(a > b);
                EXPECT_FALSE(a != b);
            }
        }
    }
}

//=============================================================================
// Edge cases with extreme values
//=============================================================================

TYPED_TEST(ComparisonOperatorsTests, ComparisonWithVeryLargeValues)
{
    EXPECT_FALSE(this->asBase(1e10) < this->asBase(1e10));
    EXPECT_FALSE(this->asBase(1e10) > this->asBase(1e10));
    EXPECT_TRUE(this->asBase(1e10) == this->asBase(1e10));
    EXPECT_TRUE(this->asBase(1e10) <= this->asBase(1e10));
    EXPECT_TRUE(this->asBase(1e10) >= this->asBase(1e10));
    EXPECT_FALSE(this->asBase(1e10) != this->asBase(1e10));

    EXPECT_LT(this->asBase(1e10), this->asBase(2e10));
    EXPECT_GT(this->asBase(2e10), this->asBase(1e10));
    EXPECT_LE(this->asBase(1e10), this->asBase(2e10));
    EXPECT_GE(this->asBase(2e10), this->asBase(1e10));
    EXPECT_NE(this->asBase(1e10), this->asBase(2e10));
}

TYPED_TEST(ComparisonOperatorsTests, ComparisonWithVerySmallValues)
{
    EXPECT_FALSE(this->asBase(1e-10) < this->asBase(1e-10));
    EXPECT_FALSE(this->asBase(1e-10) > this->asBase(1e-10));
    EXPECT_TRUE(this->asBase(1e-10) == this->asBase(1e-10));
    EXPECT_TRUE(this->asBase(1e-10) <= this->asBase(1e-10));
    EXPECT_TRUE(this->asBase(1e-10) >= this->asBase(1e-10));
    EXPECT_FALSE(this->asBase(1e-10) != this->asBase(1e-10));

    EXPECT_LT(this->asBase(1e-11), this->asBase(1e-10));
    EXPECT_GT(this->asBase(1e-10), this->asBase(1e-11));
    EXPECT_LE(this->asBase(1e-11), this->asBase(1e-10));
    EXPECT_GE(this->asBase(1e-10), this->asBase(1e-11));
    EXPECT_NE(this->asBase(1e-11), this->asBase(1e-10));
}

TYPED_TEST(ComparisonOperatorsTests, ComparisonWithVeryLargeNegativeValues)
{
    EXPECT_FALSE(this->asBase(-1e10) < this->asBase(-1e10));
    EXPECT_FALSE(this->asBase(-1e10) > this->asBase(-1e10));
    EXPECT_TRUE(this->asBase(-1e10) == this->asBase(-1e10));

    EXPECT_LT(this->asBase(-2e10), this->asBase(-1e10));
    EXPECT_GT(this->asBase(-1e10), this->asBase(-2e10));
    EXPECT_NE(this->asBase(-2e10), this->asBase(-1e10));
}

TYPED_TEST(ComparisonOperatorsTests, ComparisonWithVerySmallNegativeValues)
{
    EXPECT_FALSE(this->asBase(-1e-10) < this->asBase(-1e-10));
    EXPECT_FALSE(this->asBase(-1e-10) > this->asBase(-1e-10));
    EXPECT_TRUE(this->asBase(-1e-10) == this->asBase(-1e-10));

    EXPECT_LT(this->asBase(-1e-10), this->asBase(-5e-11));
    EXPECT_GT(this->asBase(-5e-11), this->asBase(-1e-10));
    EXPECT_NE(this->asBase(-1e-10), this->asBase(-5e-11));
}

TYPED_TEST(ComparisonOperatorsTests, ComparisonConsistencyWithCloseValues)
{
    EXPECT_LT(this->asBase(1.0000000001), this->asBase(1.0000000002));
    EXPECT_GT(this->asBase(1.0000000002), this->asBase(1.0000000001));
    EXPECT_NE(this->asBase(1.0000000001), this->asBase(1.0000000002));
    EXPECT_FALSE(this->asBase(1.0000000001) > this->asBase(1.0000000002));
    EXPECT_FALSE(this->asBase(1.0000000002) < this->asBase(1.0000000001));
    EXPECT_FALSE(this->asBase(1.0000000001) == this->asBase(1.0000000002));

    EXPECT_EQ(this->asBase(1.0000000001), this->asBase(1.0000000001));
    EXPECT_FALSE(this->asBase(1.0000000001) < this->asBase(1.0000000001));
    EXPECT_FALSE(this->asBase(1.0000000001) > this->asBase(1.0000000001));
    EXPECT_TRUE(this->asBase(1.0000000001) <= this->asBase(1.0000000001));
    EXPECT_TRUE(this->asBase(1.0000000001) >= this->asBase(1.0000000001));
}

TYPED_TEST(ComparisonOperatorsTests, ComparisonWithZeroAndNearZero)
{
    EXPECT_FALSE(this->asBase(0) < this->asBase(0));
    EXPECT_FALSE(this->asBase(0) > this->asBase(0));
    EXPECT_TRUE(this->asBase(0) == this->asBase(0));

    EXPECT_LT(this->asBase(0), this->asBase(0.00000001));
    EXPECT_GT(this->asBase(0.00000001), this->asBase(0));
    EXPECT_FALSE(this->asBase(0) > this->asBase(0.00000001));
    EXPECT_FALSE(this->asBase(0.00000001) < this->asBase(0));

    EXPECT_GT(this->asBase(0), this->asBase(-0.00000001));
    EXPECT_LT(this->asBase(-0.00000001), this->asBase(0));
    EXPECT_FALSE(this->asBase(0) < this->asBase(-0.00000001));
    EXPECT_FALSE(this->asBase(-0.00000001) > this->asBase(0));

    EXPECT_GT(this->asBase(0.00000001), this->asBase(-0.00000001));
    EXPECT_LT(this->asBase(-0.00000001), this->asBase(0.00000001));
    EXPECT_NE(this->asBase(0.00000001), this->asBase(-0.00000001));
}
