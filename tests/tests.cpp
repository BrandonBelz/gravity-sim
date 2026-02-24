#include "physics.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cmath>

TEST_CASE("Basic vector methods work as expected", "[DVector2]") {
    auto [x, y, mag] =
        GENERATE(table<double, double, double>({{0.0, 0.0, 0.0},
                                                {-20.0, 35.0, 5.0 * sqrt(65.0)},
                                                {1.0, 100.0, sqrt(10001.0)}}));
    DVector2 v(x, y);
    SECTION("Getting x and y back") {
        REQUIRE(v.get_x() == Catch::Approx(x));
        REQUIRE(v.get_y() == Catch::Approx(y));
    }
    SECTION("Getting the magnitude") {
        REQUIRE(v.magnitude() == Catch::Approx(mag));
        REQUIRE(v.magnitude() >= 0.0);
    }
}

TEST_CASE("Vector addition", "[DVector2]") {
    auto [x1, y1, x2, y2] = GENERATE(
        table<double, double, double, double>({{10.0, 20.0, 30.0, 20.0},
                                               {-45.0, 0.0, 0.0, 10.0},
                                               {60.0, -5.0, -60.0, 5.0}}));
    DVector2 v1(x1, y1);
    DVector2 v2(x2, y2);
    DVector2 v3 = v1 + v2;
    REQUIRE(v3.get_x() == Catch::Approx(x1 + x2));
    REQUIRE(v3.get_y() == Catch::Approx(y1 + y2));
}

TEST_CASE("Scalar multiplication", "[DVector2]") {
    auto [x1, y1, c] =
        GENERATE(table<double, double, double>({{10.0, 20.0, 2.0},
                                                {-45.0, 0.0, -1.0},
                                                {60.0, -5.0, 0.0},
                                                {-25.0, -6.4, 10.0}}));
    DVector2 v1(x1, y1);
    DVector2 v2 = v1 * c;
    DVector2 v3 = c * v1;
    REQUIRE(v2 == v3);
    REQUIRE(v2.get_x() == Catch::Approx(x1 * c));
    REQUIRE(v2.get_y() == Catch::Approx(y1 * c));
}
