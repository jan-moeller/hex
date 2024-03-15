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
#include "hex/detail/detail_offset_conversion.hpp"

#include <catch2/catch_all.hpp>

#include <array>

using namespace hex;
using namespace hex::detail;

TEST_CASE("offset_conversion", "[detail]")
{
    STATIC_CHECK(to_odd_q(from_odd_q(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_odd_q(from_odd_q(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_odd_q(from_odd_q(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_odd_q(from_odd_q(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_odd_q(from_odd_q(42, 99)) == std::array{42, 99});

    STATIC_CHECK(to_even_q(from_even_q(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_even_q(from_even_q(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_even_q(from_even_q(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_even_q(from_even_q(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_even_q(from_even_q(42, 99)) == std::array{42, 99});

    STATIC_CHECK(to_odd_r(from_odd_r(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_odd_r(from_odd_r(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_odd_r(from_odd_r(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_odd_r(from_odd_r(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_odd_r(from_odd_r(42, 99)) == std::array{42, 99});

    STATIC_CHECK(to_even_r(from_even_r(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_even_r(from_even_r(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_even_r(from_even_r(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_even_r(from_even_r(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_even_r(from_even_r(42, 99)) == std::array{42, 99});

    STATIC_CHECK(to_odd_s(from_odd_s(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_odd_s(from_odd_s(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_odd_s(from_odd_s(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_odd_s(from_odd_s(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_odd_s(from_odd_s(42, 99)) == std::array{42, 99});

    STATIC_CHECK(to_even_s(from_even_s(0, 0)) == std::array{0, 0});
    STATIC_CHECK(to_even_s(from_even_s(1, 0)) == std::array{1, 0});
    STATIC_CHECK(to_even_s(from_even_s(0, 1)) == std::array{0, 1});
    STATIC_CHECK(to_even_s(from_even_s(1, 2)) == std::array{1, 2});
    STATIC_CHECK(to_even_s(from_even_s(42, 99)) == std::array{42, 99});
}