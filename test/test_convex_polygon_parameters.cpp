//
// MIT License
//
// Copyright (c) 2024 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "hex/convex_polygon_parameters.hpp"

#include <catch2/catch_all.hpp>

#include <stdexcept>

using namespace hex;

TEST_CASE("convex_polygon_parameters")
{
    using namespace literals;

    SECTION("invalid")
    {
        CHECK_THROWS_AS(convex_polygon_parameters(-1_q, -1_r, -1_s, -1_q, -1_r, -1_s), std::invalid_argument);
        CHECK_THROWS_AS(convex_polygon_parameters(-4_q, -1_r, -1_s, 1_q, 2_r, 1_s), std::invalid_argument);
        CHECK_THROWS_AS(convex_polygon_parameters(-2_q, 1_r, -1_s, 2_q, 2_r, 1_s), std::invalid_argument);
        CHECK_THROWS_AS(convex_polygon_parameters(1_q, -1_r, -1_s, -2_q, 2_r, 1_s), std::invalid_argument);
        CHECK_THROWS_AS(make_regular_hexagon_parameters(-1), std::invalid_argument);
    }
    SECTION("-q triangle")
    {
        constexpr convex_polygon_parameters p(-1_q, -2_r, -1_s, 1_q, 0_r, 1_s);
        STATIC_CHECK(p.count() == 6);
        STATIC_CHECK(p.contains({-1_q, 0_r}));
        STATIC_CHECK(p.contains({1_q, -2_r}));
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(!p.contains({-1_q, -1_r}));
        STATIC_CHECK(!p.contains({-1_q, 1_r}));
        STATIC_CHECK(!p.contains({2_q, -1_r}));
    }
    SECTION("+q triangle")
    {
        constexpr convex_polygon_parameters p(-1_q, -1_r, 0_s, 1_q, 1_r, 2_s);
        STATIC_CHECK(p.count() == 6);
        STATIC_CHECK(p.contains({1_q, -1_r}));
        STATIC_CHECK(p.contains({-1_q, 1_r}));
        STATIC_CHECK(p.contains({-1_q, -1_r}));
        STATIC_CHECK(!p.contains({0_q, -2_r}));
        STATIC_CHECK(!p.contains({2_q, -1_r}));
        STATIC_CHECK(!p.contains({0_q, 1_r}));
    }
    SECTION("-r trapezoid")
    {
        constexpr convex_polygon_parameters p(-1_q, -1_r, -1_s, 1_q, 0_r, 1_s);
        STATIC_CHECK(p.count() == 5);
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(p.contains({0_q, -1_r}));
        STATIC_CHECK(p.contains({1_q, -1_r}));
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(!p.contains({0_q, 2_r}));
        STATIC_CHECK(!p.contains({2_q, -2_r}));
        STATIC_CHECK(!p.contains({2_q, 0_r}));
        STATIC_CHECK(!p.contains({-1_q, 1_r}));
    }
    SECTION("+q trapezoid")
    {
        constexpr convex_polygon_parameters p(-1_q, -1_r, 0_s, 0_q, 1_r, 2_s);
        STATIC_CHECK(p.count() == 5);
        STATIC_CHECK(p.contains({-1_q, -1_r}));
        STATIC_CHECK(p.contains({0_q, -1_r}));
        STATIC_CHECK(p.contains({0_q, 0_r}));
        STATIC_CHECK(p.contains({-1_q, 1_r}));
        STATIC_CHECK(!p.contains({0_q, -2_r}));
        STATIC_CHECK(!p.contains({1_q, -1_r}));
        STATIC_CHECK(!p.contains({0_q, 1_r}));
        STATIC_CHECK(!p.contains({-2_q, 0_r}));
    }
    SECTION("+q rhomboid")
    {
        constexpr convex_polygon_parameters p(-1_q, -1_r, -1_s, 1_q, 0_r, 2_s);
        STATIC_CHECK(p.count() == 6);
        STATIC_CHECK(p.contains({-1_q, -1_r}));
        STATIC_CHECK(p.contains({1_q, -1_r}));
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(p.contains({-1_q, 0_r}));
        STATIC_CHECK(!p.contains({0_q, -2_r}));
        STATIC_CHECK(!p.contains({2_q, -2_r}));
        STATIC_CHECK(!p.contains({2_q, 0_r}));
        STATIC_CHECK(!p.contains({-1_q, 1_r}));
    }
    SECTION("+s pentagon")
    {
        constexpr convex_polygon_parameters p(-1_q, -2_r, -1_s, 1_q, 0_r, 2_s);
        STATIC_CHECK(p.count() == 8);
        STATIC_CHECK(p.contains({0_q, -2_r}));
        STATIC_CHECK(p.contains({1_q, -2_r}));
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(p.contains({-1_q, 0_r}));
        STATIC_CHECK(p.contains({-1_q, -1_r}));
        STATIC_CHECK(!p.contains({2_q, -3_r}));
        STATIC_CHECK(!p.contains({2_q, 0_r}));
        STATIC_CHECK(!p.contains({0_q, 1_r}));
        STATIC_CHECK(!p.contains({-2_q, 0_r}));
    }
    SECTION("+r hexagon")
    {
        constexpr convex_polygon_parameters p(-2_q, -2_r, -2_s, 2_q, 1_r, 2_s);
        STATIC_CHECK(p.count() == 16);
        STATIC_CHECK(p.contains({0_q, -2_r}));
        STATIC_CHECK(p.contains({2_q, -2_r}));
        STATIC_CHECK(p.contains({2_q, 0_r}));
        STATIC_CHECK(p.contains({1_q, 1_r}));
        STATIC_CHECK(p.contains({-2_q, 1_r}));
        STATIC_CHECK(p.contains({-2_q, 0_r}));
        STATIC_CHECK(!p.contains({-2_q, -1_r}));
        STATIC_CHECK(!p.contains({2_q, -3_r}));
        STATIC_CHECK(!p.contains({-2_q, 2_r}));
        STATIC_CHECK(!p.contains({-3_q, 1_r}));
    }
    SECTION("regular hexagon")
    {
        SECTION("radius = 0")
        {
            constexpr convex_polygon_parameters p = make_regular_hexagon_parameters(0);
            STATIC_CHECK(p.count() == 1);
            STATIC_CHECK(p.contains({0_q, 0_r}));
            STATIC_CHECK(!p.contains({0_q, 1_r}));
        }
        SECTION("radius = 2")
        {
            constexpr convex_polygon_parameters p = make_regular_hexagon_parameters(2);
            STATIC_CHECK(p.count() == 19);
            STATIC_CHECK(p.contains({0_q, 0_r}));
            STATIC_CHECK(p.contains({0_q, -2_r}));
            STATIC_CHECK(p.contains({2_q, -2_r}));
            STATIC_CHECK(p.contains({2_q, 0_r}));
            STATIC_CHECK(p.contains({0_q, 2_r}));
            STATIC_CHECK(p.contains({-2_q, 2_r}));
            STATIC_CHECK(p.contains({-2_q, 0_r}));
            STATIC_CHECK(!p.contains({0_q, -3_r}));
            STATIC_CHECK(!p.contains({3_q, -3_r}));
            STATIC_CHECK(!p.contains({3_q, 0_r}));
            STATIC_CHECK(!p.contains({0_q, 3_r}));
            STATIC_CHECK(!p.contains({-3_q, 3_r}));
            STATIC_CHECK(!p.contains({-3_q, 0_r}));
        }
    }
    SECTION("regular triangle")
    {
        constexpr convex_polygon_parameters p = make_regular_triangle_parameters(1_q, 0_r, 1_s);
        STATIC_CHECK(p.count() == 6);
        STATIC_CHECK(p.contains({-1_q, 0_r}));
        STATIC_CHECK(p.contains({1_q, -2_r}));
        STATIC_CHECK(p.contains({1_q, 0_r}));
        STATIC_CHECK(!p.contains({-1_q, -1_r}));
        STATIC_CHECK(!p.contains({2_q, -2_r}));
    }
}