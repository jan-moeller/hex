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

#ifndef HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP
#define HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP

#include "detail_sqrt.hpp"

#include <algorithm>
#include <limits>

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace hex::detail
{
// These are the maximum limits the below implementation can support without overflow.
inline constexpr std::size_t isosceles_trapezoid_max_base   = std::numeric_limits<std::int32_t>::max();
inline constexpr std::size_t isosceles_trapezoid_max_height = isosceles_trapezoid_max_base;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr auto isosceles_trapezoid_size_from_base_and_height(std::size_t base, std::size_t height) -> std::size_t
{
    if (base == 0 || height == 0)
        return 0;

    assert(height <= base);
    assert(base <= isosceles_trapezoid_max_base);
    assert(height <= isosceles_trapezoid_max_height);

    std::size_t const b = base;
    std::size_t const h = height;

    // s = h * (b - h/2) + h/2
    auto const h_half = h / 2UZ;
    return h * (b - h_half) + h_half * (h % 2UZ == 0UZ); // NOLINT(readability-implicit-bool-conversion)
}

// Maximum size supported by these routines.
inline constexpr std::size_t
    isosceles_trapezoid_max_size = isosceles_trapezoid_size_from_base_and_height(isosceles_trapezoid_max_base,
                                                                                 isosceles_trapezoid_max_height);

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr auto isosceles_trapezoid_size_from_top_and_height(std::size_t top, std::size_t height) -> std::size_t
{
    return isosceles_trapezoid_size_from_base_and_height(std::max(0UZ, top + height - 1UZ), height);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr auto isosceles_trapezoid_height_from_base_and_size(std::size_t base, std::size_t size) -> std::size_t
{
    assert(base <= isosceles_trapezoid_max_base);
    assert(size <= isosceles_trapezoid_max_size);

    // h = b - sqrt((4 * b + 4) * b - 8 * s + 1) / 2 + 1/2
    std::size_t const k = floor_sqrt((4UZ * base + 4UZ) * base + 1UZ - 8 * size);
    return base - k / 2UZ + (k % 2UZ == 0UZ); // NOLINT(readability-implicit-bool-conversion)
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr auto isosceles_trapezoid_height_from_top_and_size(std::size_t top, std::size_t size) -> std::size_t
{
    assert(size <= isosceles_trapezoid_max_size);

    // 1/2 (sqrt(8 * s + (1 - 2 * t) * (1 - 2 * t)) - 2 * t + 1)
    std::size_t const k = ceil_sqrt(8UZ * size + top * (4UZ * top - 4UZ) + 1UZ) - 2UZ * top + 1UZ;
    return k / 2UZ + (k % 2UZ != 0UZ); // NOLINT(readability-implicit-bool-conversion)
}
} // namespace hex::detail

#endif // HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP
