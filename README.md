# Measurements

A small, header-only C++20 library for type-safe operations with physical quantities.

The main issue when dealing with physical quantities (`Measurements`) in code is that they are represented not only with
numerical value, but also with **units** they are measured / calculated with. Usually those units are conveyed through
variable names and comments, which is naturally fragile. This library instead implements a set of types (`Angle`,
`Distance`,`Time`, `Temperature`) with a bunch of neat features:

- Type-safe arithmetic and logical operators. Comparing `Angle` to `Temperature` won't compile.
- Explicit conversion of object to double value in specific `Units`. No more passing angle in degrees to function
  expecting radians.
- Free abstraction in terms of space. All `Measurement` types take `sizeof(double)` bytes.
- constexpr support
- Bunch of tests, covering entire public interface of `Measurement` template and derived types included in the library.

All that comes from base CRTP template, so addition of new types require minimal amount of code.

## Usage examples

### Construction from raw values

Each `Measurement` type is bound to a specific `Units` type (`Angle` -> `AngleUnits`, `Time` -> `TimeUnits`). To
construct it one has to provide an instance of the correct `Units` type and raw numerical value:

```cpp
auto range   = Distance{DistanceUnits::Meters, 1200.0};
auto heading = Angle{AngleUnits::Degrees, 45.0};
auto stamp = Time{TimeUnits::Seconds, -10}; // works with negative value
```

Passing an instance of wrong `Units` type or constructing without units at all will fail to compile:

```cpp
// does not compile, wrong units.
auto outside = Temperature{DistanceUnits::Meters, 20.0};

// also fail
Temperature outside = 20;
```

While quite unlikely to be useful in practice, custom instances of `Units` are supported. Note that type of conversion
is fixed for the specific `Units` at the type declaration level.

```cpp
auto myUnits = AngleUnits::Type{"myUnits", conversions::LinearConversion{.multiplierToBase = 50}};
auto angle = Angle{myUnits, 1};
```

### Converting to raw value

`Measurement` object itself does not have meaningful representation as a number on its own: for example you can observe
the distance between left and right edges of the monitor - that is some `Distance`, and there are other monitors of the
same model, with the equal `Distance` between the edges. However we can only represent that `Distance` as numerical
value when we specify `Units` of measure.

```cpp
double feet = range.value(DistanceUnits::Feet); // 3937.008
double radians = heading.value(AngleUnits::Radians); // pi / 4
```

Calling `value` with incompatible `Unit` type will fail to compile:

```cpp
// does not compile
heading.value(DistanceUnits::Meters);
// also does not compile, Angle seconds and Time seconds are different types.
heading.value(TimeUnits::Seconds);
```

### Comparison operations

`==` and `<=>` (three-way comparison) compare the underlying values *exactly*. That makes `Measurement` types safe as
keys in ordered containers:

```cpp
auto a = Distance{DistanceUnits::Meters, 100};
auto b = Distance{DistanceUnits::Centimeters, 10000};

bool equal = (a == b); // true — both exactly 100.0 in hub units
bool less = a < Distance{DistanceUnits::Meters, 200}; // true
```

The catch is that a value constructed in a different unit rarely produces a bit-identical `double`, so `==` is the wrong
tool for "*is this the same physical quantity*" decision:

```cpp
auto c = Distance{DistanceUnits::Meters, 1};
auto d = Distance{DistanceUnits::Feet, 3.280839895};

c == d; // likely false — different round-off
```

For such cases `fuzzyEqual` method is provided:

```cpp
c.fuzzyEqual(d); // true
c.fuzzyEqual(d * 2); // false
```

Comparison between different `Measurement` types won't work even though under-the-hood double can be compared:

```cpp
Distance{DistanceUnits::Meters, 1} == Angle{AngleUnits::Degrees, 1}; // does not compile
```

### Addition and subtraction

`+` and `-` operators require both operands to be `Measurements` of the same type:

```cpp
auto a = Distance{DistanceUnits::Meters, 100};
auto b = Distance{DistanceUnits::Feet, 100};

Distance sum = a + b;
Distance& result = a += b;
```

Mixing operands of different types fails to compile, as well as using a raw value as either side of operation:

```cpp
auto a = Distance{DistanceUnits::Meters, 100};
auto b = Distance{DistanceUnits::Feet, 100};


// All fails to compile
5 - a;
b += Angle{AngleUnits::Degrees, 90};
```

### Multiplication and division

Scaling `Measurement` by a scalar produces `Measurement` of the same type. Friend operator taking scalar as lhs operand
is also provided:

```cpp
Distance scaled = a * 2.0;
Distance alsoScaled = 2.0 * a;
```

Naturally, division works too, but only when Measurement in the lhs operand:

```cpp
auto a = Distance{DistanceUnits::Meters, 100};
Distance halved = a / 2.0;
10. / a; // fails to compile
```

Dividing one measurement by another of the same category yields a scalar, while multiplication of 2 `Measurements` is
not supported:

```cpp
double ratio = a / b;
a * b; // fails to compile
```

Division by 0 throws `std::domain_error`. That is true for both division by literal 0 and Measurement, which value in
hub units is 0:

```cpp
Distance zero{DistanceUnits::Meters, 0};

a /= 0.0; // throws std::domain_error
double r = a / zero; // throws std::domain_error
```

### Normalization

When working with angles, it is often useful to work with "trimmed" [0-359 deg] values instead of large values like 1000
degrees. That trimming operation is generalized in form of `normalized` function  which effectively returns the signed 
fractional part of the `Measurement`, expressed in the `Units` passed as an argument. 

```cpp
auto angle = Angle{AngleUnits::Degrees, 721};
angle.normalized(AngleUnits::Rotations); // results in Angle equal to 1 degree.

auto distance = Distance{DistanceUnits::Meters, 1005};
distance.normalized(DistanceUnits::Kilometers); // results in Distance equal to 5 meters

distance = Distance{DistanceUnits::Meters, 42};
distance.normalized(DistanceUnits::Meters); // results in Distance equal to 0 meters
```

### Angle functionality

Angle supports a basic set of trigonometry functions and computation of the shortest signed arc length between 2 `Angle`
objects:

```cpp
auto angle = Angle{AngleUnits::Degrees, 30};
double s = angle.sin();
double c = angle.cos();
// also tan and cot are provided
Angle arcDistance = angle.signedArc(Angle{AngleUnits::Degrees, 25}); // results in -5 degrees
```

### Time interoperability with std::chrono::duration

`Time` converts implicitly from any `std::chrono::duration` specialization, so existing chrono-based code can hand std
durations straight to functions expecting `Time`:

```cpp
Time t = std::chrono::milliseconds{1500};
Time u = std::chrono::seconds{30};
  ```

Conversion back to a chrono type is **explicit**, because silently narrowing to an integer `duration` is exactly the
kind of bug this library exists to prevent:

```cpp
auto seconds = static_cast<std::chrono::seconds>(t); // 1 (truncated)
auto millis = static_cast<std::chrono::milliseconds>(t); // 1500
```

The chrono representation's `rep` is respected, including integer truncation — `1500 ms → 1 s`, not `2 s`. For
non-truncating conversions use a floating-point rep:

```cpp
auto exact = static_cast<std::chrono::duration<double>>(t); // 1.5
```

`Time` also compares directly against chrono durations in either direction:

```cpp
t == std::chrono::seconds{1}; // true — 1.5 s compared in seconds
t > std::chrono::milliseconds{1000}; // true
t.fuzzyEqual(std::chrono::milliseconds{1500}); // true
```

The comparison uses the chrono duration's own period as the reference unit — `t == std::chrono::seconds{1}` compares
`1.5` against `1` in seconds, not against `1000` in milliseconds. This means the same `Time` can compare equal to
`std::chrono::milliseconds{1500}` and unequal to `std::chrono::seconds{1}` when the fractional part matters.
