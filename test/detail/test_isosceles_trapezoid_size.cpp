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

#include <cstdint>

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

    // Some random larger values that result in max size
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(98'303, 65'537) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(6'153'794'198L, 5'166'021'507L) == max<std::uint64_t>);

    // Maximum height possible with 32-bit / 64-bit integers
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(92'681, 92'681) == 4'294'930'221);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(6'074'000'999, 6'074'000'999)
                 == 18'446'744'070'963'499'500UZ);

    // Maximum size and base possible with 32-bit / 64-bit integers
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(max<std::uint32_t>, 1U) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_base_and_height(max<std::uint64_t>, 1UL) == max<std::uint64_t>);
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

    // Some random larger values that result in max size
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(32767, 65'537) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(987'772'692L, 5'166'021'507L) == max<std::uint64_t>);

    // Maximum height possible with 32-bit / 64-bit integers
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1, 92'681) == 4'294'930'221);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(1L, 6'074'000'999L) == 18'446'744'070'963'499'500UZ);

    // Maximum size and top possible with 32-bit / 64-bit integers
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(max<std::uint32_t>, 1U) == max<std::uint32_t>);
    STATIC_CHECK(isosceles_trapezoid_size_from_top_and_height(max<std::uint64_t>, 1UL) == max<std::uint64_t>);
}
