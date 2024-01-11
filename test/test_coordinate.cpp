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

#include "hex/coordinate.hpp"

#include <catch2/catch_all.hpp>

using namespace hex;

TEST_CASE("coordinate")
{
    using namespace literals;

    SECTION("trivially copyable")
    {
        STATIC_CHECK(std::is_trivially_copyable_v<q_coordinate<int>>);
        STATIC_CHECK(std::is_trivially_copyable_v<r_coordinate<int>>);
        STATIC_CHECK(std::is_trivially_copyable_v<s_coordinate<int>>);
    }
    SECTION("udl")
    {
        STATIC_CHECK(0_q == q_coordinate<int>(0));
        STATIC_CHECK(0_r == r_coordinate<int>(0));
        STATIC_CHECK(0_s == s_coordinate<int>(0));

        STATIC_CHECK(99999999999999_q == q_coordinate<long>(99999999999999));
        STATIC_CHECK(99999999999999_r == r_coordinate<long>(99999999999999));
        STATIC_CHECK(99999999999999_s == s_coordinate<long>(99999999999999));

        STATIC_CHECK(111'222'333_q == q_coordinate(111'222'333));
        STATIC_CHECK(111'222'333_r == r_coordinate(111'222'333));
        STATIC_CHECK(111'222'333_s == s_coordinate(111'222'333));
    }
    SECTION("default constructor")
    {
        STATIC_CHECK(0_q == q_coordinate{});
        STATIC_CHECK(0_r == r_coordinate{});
        STATIC_CHECK(0_s == s_coordinate{});
    }
    SECTION("construction from fundamental type")
    {
        STATIC_CHECK(q_coordinate{42}.value() == 42);
        STATIC_CHECK(r_coordinate{42}.value() == 42);
        STATIC_CHECK(s_coordinate{42}.value() == 42);

        STATIC_CHECK(q_coordinate<double>{42.42}.value() == 42.42);
        STATIC_CHECK(r_coordinate<double>{42.42}.value() == 42.42);
        STATIC_CHECK(s_coordinate<double>{42.42}.value() == 42.42);

        STATIC_CHECK(q_coordinate<signed char>(-1) == -1_q);
        STATIC_CHECK(r_coordinate<signed char>(-1) == -1_r);
        STATIC_CHECK(s_coordinate<signed char>(-1) == -1_s);

        static constexpr s_coordinate<signed char> foo(-(1_r).value());
        STATIC_CHECK(foo.value() == -1);
    }
    SECTION("construction from other coordinate of same axis")
    {
        STATIC_CHECK(q_coordinate<long>{42_q} == 42_q);
        STATIC_CHECK(r_coordinate<long>{42_r} == 42_r);
        STATIC_CHECK(s_coordinate<long>{42_s} == 42_s);
        STATIC_CHECK(q_coordinate<long>{42} == 42_q);
        STATIC_CHECK(r_coordinate<long>{42} == 42_r);
        STATIC_CHECK(s_coordinate<long>{42} == 42_s);
    }
    SECTION("construction from other coordinate of different axis")
    {
        constexpr auto q = q_coordinate{42_s};
        STATIC_CHECK(q == 42_q);
    }
    SECTION("conversion to underlying type")
    {
        STATIC_CHECK(static_cast<int>(42_q) == 42);
    }
    SECTION("Comparison")
    {
        STATIC_CHECK(42_q == 42_q);
        STATIC_CHECK(42_q != 12_q);
        STATIC_CHECK(42_q < 43_q);
        STATIC_CHECK(42_q <= 42_q);
        STATIC_CHECK(42_q > 41_q);
        STATIC_CHECK(42_q >= 42_q);

        STATIC_CHECK(q_coordinate<short>(42) == q_coordinate<int>(42));
    }
    SECTION("unary +")
    {
        STATIC_CHECK(+42_q == 42_q);
    }
    SECTION("unary -")
    {
        STATIC_CHECK((-42_q).value() == -42);
    }
    SECTION("binary +")
    {
        STATIC_CHECK(42_q + 12_q == 54_q);
        STATIC_CHECK(q_coordinate<int>{42} + q_coordinate<short>{12} == 54_q);
        STATIC_CHECK(q_coordinate<short>{42} + q_coordinate<int>{12} == 54_q);
    }
    SECTION("binary -")
    {
        STATIC_CHECK(42_q - 12_q == 30_q);
        STATIC_CHECK(q_coordinate<int>{42} - q_coordinate<short>{12} == 30_q);
        STATIC_CHECK(q_coordinate<short>{42} - q_coordinate<int>{12} == 30_q);
    }
    SECTION("binary *")
    {
        STATIC_CHECK(42_q * 2 == 84_q);
        STATIC_CHECK(2 * 42_q == 84_q);
    }
    SECTION("binary /")
    {
        STATIC_CHECK(42_q / 2 == 21_q);
    }
    SECTION("binary %")
    {
        STATIC_CHECK(42_q % 10 == 2_q);
    }
    SECTION("+=")
    {
        auto q = 42_q;
        CHECK((q += 2_q) == 44_q);
    }
    SECTION("-=")
    {
        auto q = 42_q;
        CHECK((q -= 2_q) == 40_q);
    }
    SECTION("*=")
    {
        auto q = 42_q;
        CHECK((q *= 2) == 84_q);
    }
    SECTION("/=")
    {
        auto q = 42_q;
        CHECK((q /= 2) == 21_q);
    }
    SECTION("%=")
    {
        auto q = 42_q;
        CHECK((q %= 10) == 2_q);
    }

    SECTION("coordinate_cast")
    {
        STATIC_CHECK(coordinate_cast<signed short>(1_q) == q_coordinate<signed short>(1));
        STATIC_CHECK(coordinate_cast<signed short>(1_r) == r_coordinate<signed short>(1));
        STATIC_CHECK(coordinate_cast<signed short>(1_s) == s_coordinate<signed short>(1));
    }
}
