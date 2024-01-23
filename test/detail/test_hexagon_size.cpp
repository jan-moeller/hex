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
#include "hex/detail/detail_hexagon_size.hpp"
#include "hex/detail/detail_isosceles_trapezoid_size.hpp"

#include <catch2/catch_all.hpp>

#include <cstdint>

using namespace hex::detail;

TEST_CASE("regular_hexagon_size", "[detail]")
{
    STATIC_CHECK(regular_hexagon_size(0) == 1);
    STATIC_CHECK(regular_hexagon_size(1) == 7);
    STATIC_CHECK(regular_hexagon_size(2) == 19);

    static constexpr auto max_radius = isosceles_trapezoid_max_base / 2;
    static constexpr auto max_size   = 1UZ + 3UZ * max_radius * (max_radius + 1UZ);
    STATIC_CHECK(regular_hexagon_size(max_radius) == max_size);
}

TEST_CASE("hexagon_size", "[detail]")
{
    SECTION("single element")
    {
        STATIC_CHECK(hexagon_size(0, 0, 0, 0, 0, 0) == 1);
        STATIC_CHECK(hexagon_size(1, 1, -2, 1, 1, -2) == 1);
        STATIC_CHECK(hexagon_size(1, -2, 1, 1, -2, 1) == 1);
        STATIC_CHECK(hexagon_size(-2, 1, 1, -2, 1, 1) == 1);
    }
    SECTION("triangle")
    {
        STATIC_CHECK(hexagon_size(-1, -1, -1, 2, 2, 2) == 10);
        STATIC_CHECK(hexagon_size(-2, -2, -2, 1, 1, 1) == 10);
    }
    SECTION("isosceles trapezoid")
    {
        STATIC_CHECK(hexagon_size(-1, 0, -1, 1, 1, 1) == 5);
    }
    SECTION("parallelogram")
    {
        STATIC_CHECK(hexagon_size(-1, -1, 0, 0, 1, 1) == 4);
        STATIC_CHECK(hexagon_size(-1, -1, -1, 1, 0, 2) == 6);
        STATIC_CHECK(hexagon_size(-1, -2, 0, 1, 1, 1) == 6);
    }
    SECTION("pentagon")
    {
        STATIC_CHECK(hexagon_size(-1, -2, -1, 1, 1, 1) == 8);
    }
    SECTION("hexagon")
    {
        STATIC_CHECK(hexagon_size(-1, -1, -1, 1, 1, 1) == 7);
        STATIC_CHECK(hexagon_size(-2, -2, -2, 2, 2, 2) == 19);
    }
    SECTION("max size")
    {
        static constexpr std::int32_t radius = isosceles_trapezoid_max_base / 2;
        static constexpr auto         size   = regular_hexagon_size(radius);
        STATIC_CHECK(hexagon_size(-radius, -radius, -radius, radius, radius, radius) == size);
        STATIC_CHECK(hexagon_size(0, 0, -radius * 3LL, 2LL * radius, 2LL * radius, -radius) == size);
    }
}

TEST_CASE("qr_to_index", "[detail]")
{
    SECTION("single element")
    {
        STATIC_CHECK(0 == qr_to_index(0, 0, 0, 0, 0, 0, 0));
        STATIC_CHECK(0 == qr_to_index(1, 1, 1, 1, -2, 1, -2));
        STATIC_CHECK(0 == qr_to_index(1, -2, 1, -2, 1, -2, 1));
        STATIC_CHECK(0 == qr_to_index(-2, 1, -2, 1, 1, 1, 1));
    }
    SECTION("triangle")
    {
        STATIC_CHECK(0 == qr_to_index(-1, -1, -1, -1, -1, 2, 2));
        STATIC_CHECK(1 == qr_to_index(-1, 0, -1, -1, -1, 2, 2));
        STATIC_CHECK(2 == qr_to_index(-1, 1, -1, -1, -1, 2, 2));
        STATIC_CHECK(3 == qr_to_index(-1, 2, -1, -1, -1, 2, 2));
        STATIC_CHECK(4 == qr_to_index(0, -1, -1, -1, -1, 2, 2));
        STATIC_CHECK(5 == qr_to_index(0, 0, -1, -1, -1, 2, 2));
        STATIC_CHECK(6 == qr_to_index(0, 1, -1, -1, -1, 2, 2));
        STATIC_CHECK(7 == qr_to_index(1, -1, -1, -1, -1, 2, 2));
        STATIC_CHECK(8 == qr_to_index(1, 0, -1, -1, -1, 2, 2));
        STATIC_CHECK(9 == qr_to_index(2, -1, -1, -1, -1, 2, 2));

        STATIC_CHECK(0 == qr_to_index(-2, 1, -2, -2, -2, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 0, -2, -2, -2, 1, 1));
        STATIC_CHECK(2 == qr_to_index(-1, 1, -2, -2, -2, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, -1, -2, -2, -2, 1, 1));
        STATIC_CHECK(4 == qr_to_index(0, 0, -2, -2, -2, 1, 1));
        STATIC_CHECK(5 == qr_to_index(0, 1, -2, -2, -2, 1, 1));
        STATIC_CHECK(6 == qr_to_index(1, -2, -2, -2, -2, 1, 1));
        STATIC_CHECK(7 == qr_to_index(1, -1, -2, -2, -2, 1, 1));
        STATIC_CHECK(8 == qr_to_index(1, 0, -2, -2, -2, 1, 1));
        STATIC_CHECK(9 == qr_to_index(1, 1, -2, -2, -2, 1, 1));
    }
    SECTION("isosceles trapezoid")
    {
        STATIC_CHECK(0 == qr_to_index(-1, 0, -1, 0, -1, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 1, -1, 0, -1, 1, 1));
        STATIC_CHECK(2 == qr_to_index(0, 0, -1, 0, -1, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, 1, -1, 0, -1, 1, 1));
        STATIC_CHECK(4 == qr_to_index(1, 0, -1, 0, -1, 1, 1));
    }
    SECTION("parallelogram")
    {
        STATIC_CHECK(0 == qr_to_index(-1, 0, -1, -1, 0, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 1, -1, -1, 0, 1, 1));
        STATIC_CHECK(2 == qr_to_index(0, -1, -1, -1, 0, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, 0, -1, -1, 0, 1, 1));

        STATIC_CHECK(0 == qr_to_index(-1, -1, -1, -1, -1, 0, 2));
        STATIC_CHECK(1 == qr_to_index(-1, 0, -1, -1, -1, 0, 2));
        STATIC_CHECK(2 == qr_to_index(0, -1, -1, -1, -1, 0, 2));
        STATIC_CHECK(3 == qr_to_index(0, 0, -1, -1, -1, 0, 2));
        STATIC_CHECK(4 == qr_to_index(1, -1, -1, -1, -1, 0, 2));
        STATIC_CHECK(5 == qr_to_index(1, 0, -1, -1, -1, 0, 2));

        STATIC_CHECK(0 == qr_to_index(-1, 0, -1, -2, 0, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 1, -1, -2, 0, 1, 1));
        STATIC_CHECK(2 == qr_to_index(0, -1, -1, -2, 0, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, 0, -1, -2, 0, 1, 1));
        STATIC_CHECK(4 == qr_to_index(1, -2, -1, -2, 0, 1, 1));
        STATIC_CHECK(5 == qr_to_index(1, -1, -1, -2, 0, 1, 1));
    }
    SECTION("pentagon")
    {
        STATIC_CHECK(0 == qr_to_index(-1, 0, -1, -2, -1, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 1, -1, -2, -1, 1, 1));
        STATIC_CHECK(2 == qr_to_index(0, -1, -1, -2, -1, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, 0, -1, -2, -1, 1, 1));
        STATIC_CHECK(4 == qr_to_index(0, 1, -1, -2, -1, 1, 1));
        STATIC_CHECK(5 == qr_to_index(1, -2, -1, -2, -1, 1, 1));
        STATIC_CHECK(6 == qr_to_index(1, -1, -1, -2, -1, 1, 1));
        STATIC_CHECK(7 == qr_to_index(1, 0, -1, -2, -1, 1, 1));
    }
    SECTION("hexagon")
    {
        STATIC_CHECK(0 == qr_to_index(-1, 0, -1, -1, -1, 1, 1));
        STATIC_CHECK(1 == qr_to_index(-1, 1, -1, -1, -1, 1, 1));
        STATIC_CHECK(2 == qr_to_index(0, -1, -1, -1, -1, 1, 1));
        STATIC_CHECK(3 == qr_to_index(0, 0, -1, -1, -1, 1, 1));
        STATIC_CHECK(4 == qr_to_index(0, 1, -1, -1, -1, 1, 1));
        STATIC_CHECK(5 == qr_to_index(1, -1, -1, -1, -1, 1, 1));
        STATIC_CHECK(6 == qr_to_index(1, 0, -1, -1, -1, 1, 1));
    }
    SECTION("max size")
    {
        static constexpr std::int32_t radius = isosceles_trapezoid_max_base / 2;
        static constexpr auto         size   = regular_hexagon_size(radius);
        STATIC_CHECK(0 == qr_to_index(-radius, 0, -radius, -radius, -radius, radius, radius));
        STATIC_CHECK(1 == qr_to_index(-radius, 1, -radius, -radius, -radius, radius, radius));
        STATIC_CHECK(size - 1 == qr_to_index(radius, 0, -radius, -radius, -radius, radius, radius));

        STATIC_CHECK(0 == qr_to_index(0, radius, 0, 0, -radius * 3LL, 2LL * radius, -radius));
        STATIC_CHECK(1 == qr_to_index(0, radius + 1, 0, 0, -radius * 3LL, 2LL * radius, -radius));
        STATIC_CHECK(size - 1 == qr_to_index(2 * radius, radius, 0, 0, -radius * 3LL, 2LL * radius, -radius));
    }
}
