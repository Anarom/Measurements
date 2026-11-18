// SPDX-License-Identifier: MIT
#include "GenericTestSuite.hpp"

template <class T>
class ArithmeticOperatorsTests : public GenericTestSuite<T>
{
};

TYPED_TEST_SUITE(ArithmeticOperatorsTests, Types);

template <class T1, class T2>
concept has_addition = requires(T1 a, T2 b) { a + b; };

template <class T1, class T2>
concept has_subtraction = requires(T1 a, T2 b) { a - b; };

template <class T1, class T2>
concept has_multiplication = requires(T1 a, T2 b) { a * b; };

template <class T1, class T2>
concept has_division = requires(T1 a, T2 b) { a / b; };

template <class T>
concept has_unary_minus = requires(T a) { -a; };

//=============================================================================
// Tests for operations that should NOT be supported
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedAdditionWithDouble)
{
    EXPECT_FALSE((has_addition<TypeParam, double>));
}

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedSubtractionWithDouble)
{
    EXPECT_FALSE((has_subtraction<TypeParam, double>));
}

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedMultiplicationOfTwoMeasurements)
{
    EXPECT_FALSE((has_multiplication<TypeParam, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedDoubleDividedByMeasurement)
{
    EXPECT_FALSE((has_division<double, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedAdditionOfDifferentMeasurementTypes)
{
    using OtherType = std::conditional_t<std::is_same_v<TypeParam, Angle>, Distance, Angle>;
    EXPECT_FALSE((has_addition<TypeParam, OtherType>));
    EXPECT_FALSE((has_addition<OtherType, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, UnsupportedSubtractionOfDifferentMeasurementTypes)
{
    using OtherType = std::conditional_t<std::is_same_v<TypeParam, Angle>, Distance, Angle>;
    EXPECT_FALSE((has_subtraction<TypeParam, OtherType>));
    EXPECT_FALSE((has_subtraction<OtherType, TypeParam>));
}

//=============================================================================
// Tests for operations that SHOULD be supported
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, SupportedAdditionOfSameType)
{
    EXPECT_TRUE((has_addition<TypeParam, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, SupportedSubtractionOfSameType)
{
    EXPECT_TRUE((has_subtraction<TypeParam, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, SupportedUnaryMinus)
{
    EXPECT_TRUE(has_unary_minus<TypeParam>);
}

TYPED_TEST(ArithmeticOperatorsTests, SupportedMultiplicationByDouble)
{
    EXPECT_TRUE((has_multiplication<TypeParam, double>));
    EXPECT_TRUE((has_multiplication<double, TypeParam>));
}

TYPED_TEST(ArithmeticOperatorsTests, SupportedDivisionByDouble)
{
    EXPECT_TRUE((has_division<TypeParam, double>));
}

TYPED_TEST(ArithmeticOperatorsTests, SupportedDivisionByMeasurement)
{
    EXPECT_TRUE((has_division<TypeParam, TypeParam>));
}

//=============================================================================
// Addition operator (+)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, AdditionOfTwoMeasurements)
{
    EXPECT_EQ(this->asBase(1) + this->asBase(2), this->asBase(3));
    EXPECT_EQ(this->asBase(2) + this->asBase(3), this->asBase(5));
    EXPECT_EQ(this->asBase(0.5) + this->asBase(0.5), this->asBase(1));
    EXPECT_EQ(this->asBase(0) + this->asBase(5), this->asBase(5));
    EXPECT_EQ(this->asBase(-1) + this->asBase(1), this->asBase(0));
    EXPECT_EQ(this->asBase(-5) + this->asBase(-2), this->asBase(-7));
    EXPECT_EQ(this->asBase(1.5) + this->asBase(2.5), this->asBase(4));
    EXPECT_TRUE((this->asBase(0.1) + this->asBase(0.2)).fuzzyEqual(this->asBase(0.3)));
}

TYPED_TEST(ArithmeticOperatorsTests, AdditionWithZero)
{
    EXPECT_EQ(this->asBase(1) + this->asBase(0), this->asBase(1));
    EXPECT_EQ(this->asBase(0) + this->asBase(1), this->asBase(1));
    EXPECT_EQ(this->asBase(-10) + this->asBase(0), this->asBase(-10));
    EXPECT_EQ(this->asBase(0) + this->asBase(0), this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, AdditionIsCommutative)
{
    EXPECT_EQ(this->asBase(7) + this->asBase(3), this->asBase(3) + this->asBase(7));
    EXPECT_EQ(this->asBase(1) + this->asBase(2), this->asBase(2) + this->asBase(1));
    EXPECT_EQ(this->asBase(-5) + this->asBase(3), this->asBase(3) + this->asBase(-5));
    EXPECT_EQ(this->asBase(0.5) + this->asBase(0.25), this->asBase(0.25) + this->asBase(0.5));
}

TYPED_TEST(ArithmeticOperatorsTests, AdditionIsAssociative)
{
    EXPECT_EQ((this->asBase(2) + this->asBase(3)) + this->asBase(4),
              this->asBase(2) + (this->asBase(3) + this->asBase(4)));

    EXPECT_EQ((this->asBase(-1) + this->asBase(2)) + this->asBase(-3),
              this->asBase(-1) + (this->asBase(2) + this->asBase(-3)));
}

//=============================================================================
// Subtraction operator (-)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, SubtractionOfTwoMeasurements)
{
    EXPECT_EQ(this->asBase(2) - this->asBase(1), this->asBase(1));
    EXPECT_EQ(this->asBase(5) - this->asBase(3), this->asBase(2));
    EXPECT_EQ(this->asBase(0.5) - this->asBase(0.25), this->asBase(0.25));
    EXPECT_EQ(this->asBase(0) - this->asBase(5), this->asBase(-5));
    EXPECT_EQ(this->asBase(-1) - this->asBase(-2), this->asBase(1));
    EXPECT_EQ(this->asBase(-5) - this->asBase(2), this->asBase(-7));
    EXPECT_EQ(this->asBase(1.5) - this->asBase(0.5), this->asBase(1));
}

TYPED_TEST(ArithmeticOperatorsTests, SubtractionWithZero)
{
    EXPECT_EQ(this->asBase(1) - this->asBase(0), this->asBase(1));
    EXPECT_EQ(this->asBase(0) - this->asBase(1), this->asBase(-1));
    EXPECT_EQ(this->asBase(-10) - this->asBase(0), this->asBase(-10));
    EXPECT_EQ(this->asBase(0) - this->asBase(0), this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, SubtractionOfSelf)
{
    EXPECT_EQ(this->asBase(1) - this->asBase(1), this->asBase(0));
    EXPECT_EQ(this->asBase(5) - this->asBase(5), this->asBase(0));
    EXPECT_EQ(this->asBase(-3) - this->asBase(-3), this->asBase(0));
    EXPECT_EQ(this->asBase(0.5) - this->asBase(0.5), this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, SubtractionRelationshipWithAddition)
{
    EXPECT_EQ(this->asBase(5) - this->asBase(3), this->asBase(5) + this->asBase(-3));
    EXPECT_EQ(this->asBase(7) - this->asBase(2), this->asBase(7) + this->asBase(-2));
    EXPECT_EQ(this->asBase(-5) - this->asBase(3), this->asBase(-5) + this->asBase(-3));
}

//=============================================================================
// Unary minus operator (-)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, UnaryMinus)
{
    EXPECT_EQ(-this->asBase(1), this->asBase(-1));
    EXPECT_EQ(-this->asBase(-1), this->asBase(1));
    EXPECT_EQ(-this->asBase(0), this->asBase(0));
    EXPECT_EQ(-this->asBase(0.5), this->asBase(-0.5));
    EXPECT_EQ(-(-this->asBase(2)), this->asBase(2));
}

TYPED_TEST(ArithmeticOperatorsTests, UnaryMinusConsistency)
{
    // -(-a) == a
    EXPECT_EQ(-(-this->asBase(5)), this->asBase(5));
    EXPECT_EQ(-(-this->asBase(-3)), this->asBase(-3));

    // a - b == a + (-b)
    EXPECT_EQ(this->asBase(5) - this->asBase(3), this->asBase(5) + (-this->asBase(3)));
    EXPECT_EQ(this->asBase(-2) - this->asBase(1), this->asBase(-2) + (-this->asBase(1)));
}

//=============================================================================
// Multiplication with double (Measurement * double)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationByDouble)
{
    EXPECT_EQ(this->asBase(1) * 2.0, this->asBase(2));
    EXPECT_EQ(this->asBase(2) * 3.0, this->asBase(6));
    EXPECT_EQ(this->asBase(0.5) * 2.0, this->asBase(1));
    EXPECT_EQ(this->asBase(-3) * 2.0, this->asBase(-6));
    EXPECT_EQ(this->asBase(5) * 0.5, this->asBase(2.5));
    EXPECT_EQ(this->asBase(0) * 2.0, this->asBase(0));
    EXPECT_EQ(this->asBase(10) * 0.0, this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationByOne)
{
    EXPECT_EQ(this->asBase(1) * 1.0, this->asBase(1));
    EXPECT_EQ(this->asBase(5) * 1.0, this->asBase(5));
    EXPECT_EQ(this->asBase(-3) * 1.0, this->asBase(-3));
    EXPECT_EQ(this->asBase(0.5) * 1.0, this->asBase(0.5));
}

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationByZero)
{
    EXPECT_EQ(this->asBase(1) * 0.0, this->asBase(0));
    EXPECT_EQ(this->asBase(100) * 0.0, this->asBase(0));
    EXPECT_EQ(this->asBase(-5) * 0.0, this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationByNegative)
{
    EXPECT_EQ(this->asBase(1) * -2.0, this->asBase(-2));
    EXPECT_EQ(this->asBase(-2) * -2.0, this->asBase(4));
    EXPECT_EQ(this->asBase(0.5) * -2.0, this->asBase(-1));
    EXPECT_EQ(this->asBase(-0.5) * -2.0, this->asBase(1));
}

//=============================================================================
// Multiplication with double (double * Measurement)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationDoubleByMeasurement)
{
    EXPECT_EQ(2.0 * this->asBase(1), this->asBase(2));
    EXPECT_EQ(3.0 * this->asBase(2), this->asBase(6));
    EXPECT_EQ(2.0 * this->asBase(0.5), this->asBase(1));
    EXPECT_EQ(2.0 * this->asBase(-3), this->asBase(-6));
    EXPECT_EQ(0.5 * this->asBase(5), this->asBase(2.5));
    EXPECT_EQ(0.0 * this->asBase(1), this->asBase(0));
    EXPECT_EQ(0.0 * this->asBase(10), this->asBase(0));
}

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationIsCommutative)
{
    // a * b == b * a (for double and Measurement)
    EXPECT_EQ(this->asBase(1) * 2.0, 2.0 * this->asBase(1));
    EXPECT_EQ(this->asBase(3) * 4.0, 4.0 * this->asBase(3));
    EXPECT_EQ(this->asBase(0.5) * 3.0, 3.0 * this->asBase(0.5));
    EXPECT_EQ(this->asBase(-2) * 5.0, 5.0 * this->asBase(-2));
}

//=============================================================================
// Division operators
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, DivisionByDouble)
{
    EXPECT_EQ(this->asBase(2) / 2.0, this->asBase(1));
    EXPECT_EQ(this->asBase(6) / 2.0, this->asBase(3));
    EXPECT_EQ(this->asBase(1) / 2.0, this->asBase(0.5));
    EXPECT_EQ(this->asBase(-5) / 2.0, this->asBase(-2.5));
    EXPECT_EQ(this->asBase(0) / 2.0, this->asBase(0));
    EXPECT_EQ(this->asBase(10) / 4.0, this->asBase(2.5));
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionByOne)
{
    // Dividing by 1 should not change value
    EXPECT_EQ(this->asBase(1) / 1.0, this->asBase(1));
    EXPECT_EQ(this->asBase(5) / 1.0, this->asBase(5));
    EXPECT_EQ(this->asBase(-3) / 1.0, this->asBase(-3));
    EXPECT_EQ(this->asBase(0.5) / 1.0, this->asBase(0.5));
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionByZero)
{
    EXPECT_THROW(this->asBase(1) / 0.0, std::domain_error);
    EXPECT_THROW(this->asBase(5) / 0.0, std::domain_error);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionByDoubleConsistencyWithMultiplication)
{
    // a / b == a * (1/b)
    EXPECT_EQ(this->asBase(6) / 3.0, this->asBase(6) * (1.0 / 3.0));
    EXPECT_EQ(this->asBase(5) / 2.0, this->asBase(5) * 0.5);
    EXPECT_EQ(this->asBase(-10) / 2.0, this->asBase(-10) * 0.5);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionOfTwoMeasurements)
{
    EXPECT_EQ(this->asBase(2) / this->asBase(1), 2.0);
    EXPECT_EQ(this->asBase(6) / this->asBase(2), 3.0);
    EXPECT_EQ(this->asBase(1) / this->asBase(2), 0.5);
    EXPECT_EQ(this->asBase(0.5) / this->asBase(1), 0.5);
    EXPECT_EQ(this->asBase(-5) / this->asBase(2), -2.5);
    EXPECT_EQ(this->asBase(-10) / this->asBase(-2), 5.0);
    EXPECT_EQ(this->asBase(0.1) / this->asBase(0.2), 0.5);
    EXPECT_EQ(this->asBase(2.5) / this->asBase(0.5), 5.0);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionByItself)
{
    EXPECT_EQ(this->asBase(1) / this->asBase(1), 1.0);
    EXPECT_EQ(this->asBase(5) / this->asBase(5), 1.0);
    EXPECT_EQ(this->asBase(-3) / this->asBase(-3), 1.0);
    EXPECT_EQ(this->asBase(0.5) / this->asBase(0.5), 1.0);
    EXPECT_EQ(this->asBase(100) / this->asBase(100), 1.0);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionByZeroMeasurement)
{
    EXPECT_THROW(this->asBase(1) / this->asBase(0), std::domain_error);
    EXPECT_THROW(this->asBase(5) / this->asBase(0), std::domain_error);
    EXPECT_THROW(this->asBase(0) / this->asBase(0), std::domain_error);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionIsNotCommutative)
{
    EXPECT_NE(this->asBase(2) / this->asBase(1), this->asBase(1) / this->asBase(2));
    EXPECT_NE(this->asBase(4) / this->asBase(2), this->asBase(2) / this->asBase(4));
    EXPECT_NE(this->asBase(-10) / this->asBase(2), this->asBase(2) / this->asBase(-10));
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionsConsistencyWithMultiplication)
{
    // (a / b) * b == a
    EXPECT_EQ(this->asBase(6) / this->asBase(2) * this->asBase(2), this->asBase(6));
    EXPECT_EQ(this->asBase(5) / this->asBase(0.5) * this->asBase(0.5), this->asBase(5));
    EXPECT_EQ(this->asBase(-10) / this->asBase(5) * this->asBase(5), this->asBase(-10));
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionChaining)
{
    auto result1 = this->asBase(100) / this->asBase(2) / 5.;
    auto result2 = this->asBase(100) / (this->asBase(2) * 5.);
    EXPECT_DOUBLE_EQ(result1, result2);

    auto result3 = this->asBase(-10) / this->asBase(2) / -1.;
    auto result4 = this->asBase(-10) / (this->asBase(2) * -1.);
    EXPECT_DOUBLE_EQ(result3, result4);
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionWithVerySmallValues)
{
    double result = this->asBase(1e-8) / this->asBase(1e-8);
    EXPECT_DOUBLE_EQ(result, 1.0);

    result = this->asBase(1e-8) / this->asBase(2);
    EXPECT_DOUBLE_EQ(result, 5e-9);
}

//=============================================================================
// Compound assignment operators (+=, -=, *=, /=)
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, AdditionAssignment)
{
    auto value = this->asBase(1);
    value += this->asBase(2);
    EXPECT_EQ(value, this->asBase(3));

    value = this->asBase(-5);
    value += this->asBase(3);
    EXPECT_EQ(value, this->asBase(-2));

    value = this->asBase(0.5);
    value += this->asBase(0.25);
    EXPECT_EQ(value, this->asBase(0.75));
}

TYPED_TEST(ArithmeticOperatorsTests, SubtractionAssignment)
{
    auto value = this->asBase(5);
    value -= this->asBase(2);
    EXPECT_EQ(value, this->asBase(3));

    value = this->asBase(-1);
    value -= this->asBase(2);
    EXPECT_EQ(value, this->asBase(-3));

    value = this->asBase(1.5);
    value -= this->asBase(0.5);
    EXPECT_EQ(value, this->asBase(1.0));
}

TYPED_TEST(ArithmeticOperatorsTests, MultiplicationAssignment)
{
    auto value = this->asBase(2);
    value *= 3.0;
    EXPECT_EQ(value, this->asBase(6));

    value = this->asBase(-3);
    value *= 2.0;
    EXPECT_EQ(value, this->asBase(-6));

    value = this->asBase(0.5);
    value *= 4.0;
    EXPECT_EQ(value, this->asBase(2.0));
}

TYPED_TEST(ArithmeticOperatorsTests, DivisionAssignment)
{
    auto value = this->asBase(6);
    value /= 2.0;
    EXPECT_EQ(value, this->asBase(3));

    value = this->asBase(-10);
    value /= 2.0;
    EXPECT_EQ(value, this->asBase(-5));

    value = this->asBase(1);
    value /= 2.0;
    EXPECT_EQ(value, this->asBase(0.5));
}

//=============================================================================
// Complex expressions
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, ComplexExpression1)
{
    // (a + b) * c - d / e
    auto result = (this->asBase(1) + this->asBase(2)) * 3.0 - this->asBase(4) / 2.0;
    EXPECT_EQ(result, this->asBase(3.0 * 3.0 - 2.0));
}

TYPED_TEST(ArithmeticOperatorsTests, ComplexExpression2)
{
    // -a * 2.0 + b / 2.0 - (-c)
    auto result = -this->asBase(2) * 2.0 + this->asBase(4) / 2.0 - (-this->asBase(3));
    EXPECT_EQ(result, this->asBase(-4.0 + 2.0 - -3.0));
}

TYPED_TEST(ArithmeticOperatorsTests, ComplexExpressionWithParentheses)
{
    // (a - b)  * 2.0 + (c - d) / 2.0
    auto result = (this->asBase(5) - this->asBase(2)) * 2.0
                  + (this->asBase(3) + this->asBase(1)) / 2.0;
    EXPECT_EQ(result, this->asBase(3.0 * 2.0 + 4.0 / 2.0));
}

TYPED_TEST(ArithmeticOperatorsTests, ComplexExpressionWithDivisions)
{
    // (a + b) / (c - d)
    double result = (this->asBase(5) + this->asBase(3)) / (this->asBase(4) - this->asBase(2));
    EXPECT_DOUBLE_EQ(result, 8.0 / 2.0);

    // (a * 2.0) / (b + c)
    double result2 = (this->asBase(3) * 2.0) / (this->asBase(2) + this->asBase(1));
    EXPECT_DOUBLE_EQ(result2, 6.0 / 3.0);
}

TYPED_TEST(ArithmeticOperatorsTests, ComplexExpressionWithMixedTypes)
{
    // ((a + b) * 2.0) / (c - d)
    double result = ((this->asBase(5) + this->asBase(3)) * 2.0)
                    / (this->asBase(4) - this->asBase(2));
    EXPECT_DOUBLE_EQ(result, (8.0 * 2.0) / 2.0);

    // (a * 2.0 + b) / c
    double result2 = (this->asBase(3) * 2.0 + this->asBase(5)) / this->asBase(2);
    EXPECT_DOUBLE_EQ(result2, (6.0 + 5.0) / 2.0);
}

TYPED_TEST(ArithmeticOperatorsTests, ChainedOperations)
{
    // Chained additions
    auto result = this->asBase(1) + this->asBase(2) + this->asBase(3) + this->asBase(4);
    EXPECT_EQ(result, this->asBase(10));

    // Chained subtractions
    result = this->asBase(10) - this->asBase(5) - this->asBase(2) - this->asBase(1);
    EXPECT_EQ(result, this->asBase(2));

    // Chained multiplications (with double)
    result = this->asBase(1) * 2.0 * 3.0 * 4.0;
    EXPECT_EQ(result, this->asBase(24));

    // Chained divisions (by double)
    result = this->asBase(100) / 2.0 / 5.0 / 2.0;
    EXPECT_EQ(result, this->asBase(5));

    // Chained divisions of measurements - requires parentheses
    double ratio = this->asBase(100) / this->asBase(2) / 5.;
    EXPECT_DOUBLE_EQ(ratio, 10.0);

    // Alternative: compute step by step
    double step1  = this->asBase(100) / this->asBase(2);
    double ratio2 = step1 / 5.;
    EXPECT_DOUBLE_EQ(ratio2, 10.0);
}

//=============================================================================
// Edge cases
//=============================================================================

TYPED_TEST(ArithmeticOperatorsTests, VerySmallValues)
{
    auto small   = this->asBase(1e-10);
    auto smaller = this->asBase(1e-11);

    // Addition and subtraction
    EXPECT_DOUBLE_EQ(this->toBase(small + small), 2e-10);
    EXPECT_DOUBLE_EQ(this->toBase(small - smaller), 9e-11);

    // Multiplication and division by double
    EXPECT_DOUBLE_EQ(this->toBase(small * 1e10), 1.0);
    EXPECT_DOUBLE_EQ(this->toBase(small / 1e-10), 1.0);

    // Division of two measurements (returns double)
    EXPECT_DOUBLE_EQ(small / smaller, 10.0);

    // Additional edge cases
    EXPECT_DOUBLE_EQ(this->toBase(small + small), this->toBase(small * 2));
    EXPECT_DOUBLE_EQ(this->toBase(smaller * 2.0), 2e-11);
    EXPECT_DOUBLE_EQ(this->toBase(small / 2.0), 5e-11);
}

TYPED_TEST(ArithmeticOperatorsTests, VeryLargeValues)
{
    auto large  = this->asBase(1e10);
    auto larger = this->asBase(2e10);

    EXPECT_EQ(large + large, larger);
    EXPECT_EQ(larger - large, large);
    EXPECT_EQ(large * 2.0, larger);
    EXPECT_EQ(larger / 2.0, large);
    EXPECT_EQ(larger / large, 2.0);
}

TYPED_TEST(ArithmeticOperatorsTests, MixedSignOperations)
{
    EXPECT_EQ(this->asBase(-5) + this->asBase(3), this->asBase(-2));
    EXPECT_EQ(this->asBase(-5) - this->asBase(3), this->asBase(-8));
    EXPECT_EQ(this->asBase(-5) * -2.0, this->asBase(10));
    EXPECT_EQ(this->asBase(-5) / -2.0, this->asBase(2.5));
    EXPECT_EQ(this->asBase(-5) / this->asBase(2), -2.5);
    EXPECT_EQ(this->asBase(5) / this->asBase(-2), -2.5);
}
