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
#include "hex/detail/detail_isosceles_trapezoid_size.hpp"

#include <catch2/catch_all.hpp>

#include <limits>

using namespace hex::detail;
template<typename T>
using nl = std::numeric_limits<T>;
template<typename T>
inline constexpr T max = nl<T>::max();

TEST_CASE("isosceles_trapezoid_size_from_base_and_height", "[detail]")
{
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(0, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(1, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(1, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(2, 1) == 2);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(3, 1) == 3);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(2, 2) == 3);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(3, 2) == 5);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(3, 3) == 6);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(20, 15) == 195);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(98'303, 65'537) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(92'681, 92'681) == 4'294'930'221);
    STATIC_CHECK(
        isosceles_trapezoid_size_from_base_and_height(isosceles_trapezoid_max_base, isosceles_trapezoid_max_height)
        == isosceles_trapezoid_max_size);
}

TEST_CASE("isosceles_trapezoid_size_from_top_and_height", "[detail]")
{
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(0, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 2) == 3);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 3) == 6);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(2, 1) == 2);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(3, 1) == 3);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(2, 2) == 5);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(3, 2) == 7);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(3, 3) == 12);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(6, 15) == 195);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(32'767, 65'537) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 92'681) == 4'294'930'221);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1L, isosceles_trapezoid_max_height)
                 == isosceles_trapezoid_max_size);
}

TEST_CASE("isosceles_trapezoid_height_from_base_and_size", "[detail]")
{
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(0, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(1, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(1, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(2, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(2, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(2, 2) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(2, 3) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 2) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 3) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 4) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 5) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(3, 6) == 3);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(20, 195) == 15);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(98'303, max<std::uint32_t>) == 65'537);
    STATIC_CHECK(isosceles_trapezoid_height_from_base_and_size(92'681, 4'294'930'221) == 92'681);
    STATIC_CHECK(
        isosceles_trapezoid_height_from_base_and_size(isosceles_trapezoid_max_base, isosceles_trapezoid_max_size)
        == isosceles_trapezoid_max_height);
}

TEST_CASE("isosceles_trapezoid_height_from_top_and_size", "[detail]")
{
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(1, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(1, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(2, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(2, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(2, 2) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(2, 3) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 0) == 0);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 1) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 2) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 3) == 1);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 4) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 5) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 6) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 7) == 2);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(3, 12) == 3);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(6, 195) == 15);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(32'767, max<std::uint32_t>) == 65'537);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(1, 4'294'930'221) == 92'681);
    STATIC_CHECK(isosceles_trapezoid_height_from_top_and_size(1L, isosceles_trapezoid_max_size)
                 == isosceles_trapezoid_max_height);
}