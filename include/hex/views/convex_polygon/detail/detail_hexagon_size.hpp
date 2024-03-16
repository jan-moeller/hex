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

#ifndef HEX_DETAIL_HEXAGON_SIZE_HPP
#define HEX_DETAIL_HEXAGON_SIZE_HPP

#include "hex/views/convex_polygon/detail/detail_isosceles_trapezoid_size.hpp"

#include <algorithm>
#include <array>

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace hex::detail
{
// These algorithms work as long as max(q_max-q_min, r_max-r_min, s_max-s_min) <= isosceles_trapezoid_max_base

// Computes the size of a regular hexagon.
constexpr auto regular_hexagon_size(std::size_t radius) -> std::size_t
{
    return 1UZ + 3UZ * radius * (radius + 1UZ);
}

// Computes the number of tiles in a hexagon assuming tight bounds.
constexpr auto hexagon_size(std::int64_t q_min, // NOLINT(bugprone-easily-swappable-parameters)
                            std::int64_t r_min, // NOLINT(bugprone-easily-swappable-parameters)
                            std::int64_t s_min, // NOLINT(bugprone-easily-swappable-parameters)
                            std::int64_t q_max, // NOLINT(bugprone-easily-swappable-parameters)
                            std::int64_t r_max, // NOLINT(bugprone-easily-swappable-parameters)
                            std::int64_t s_max  // NOLINT(bugprone-easily-swappable-parameters)
                            ) -> std::size_t
{
    // Subdivide into left trapezoidal, right trapezoidal, and center rectangular part
    auto const        left_qmax     = std::min(-r_min - s_max, -r_max - s_min);
    auto const        right_qmin    = std::max(std::max(-r_min - s_max, -r_max - s_min), left_qmax + 1);
    std::size_t const middle_height = right_qmin - left_qmax - 1;

    // Compute left trapezoid bounds
    std::size_t const left_height = left_qmax - q_min + 1;
    std::size_t const left_base   = static_cast<std::size_t>(r_max) + left_qmax + s_max + 1;
    std::size_t const left_size   = isosceles_trapezoid_size_from_base_and_height(left_base, left_height);

    // Compute right trapezoid bounds
    std::size_t const right_height = static_cast<std::size_t>(q_max) - right_qmin + 1;
    std::size_t const right_base   = -r_min - right_qmin - s_min + 1;
    std::size_t const right_size   = isosceles_trapezoid_size_from_base_and_height(right_base, right_height);

    // Compute middle parallelogram
    std::size_t const middle_size = middle_height * left_base;

    return left_size + middle_size + right_size;
}

constexpr auto qr_to_index(std::int64_t q,     // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t r,     // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t q_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t r_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t s_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t r_max, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t s_max  // NOLINT(bugprone-easily-swappable-parameters)
                           ) -> std::size_t
{
    assert(q >= q_min);
    assert(r >= r_min && r <= r_max);

    // The bounds of the "left" part between qmin and q-1 (fix rmin and rmax to be tight given qmax)
    std::int64_t const left_q_max = q - 1;
    std::int64_t const left_r_max = r_max;
    std::int64_t const left_s_max = s_max;
    std::int64_t const left_q_min = q_min;
    std::int64_t const left_r_min = (-r_min - left_s_max <= left_q_max) ? r_min : -left_q_max - left_s_max;
    std::int64_t const left_s_min = (-left_r_max - s_min <= left_q_max) ? s_min : -left_q_max - left_r_max;

    bool const valid = left_q_max >= q_min;
    auto const b = valid ? hexagon_size(left_q_min, left_r_min, left_s_min, left_q_max, left_r_max, left_s_max) : 0UZ;
    std::int64_t const r_min2 = std::max(r_min, -s_max - q);
    std::int64_t const c      = r - r_min2;
    return b + c;
}

constexpr auto index_to_qr(std::size_t  idx,   // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t q_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t r_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t s_min, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t r_max, // NOLINT(bugprone-easily-swappable-parameters)
                           std::int64_t s_max  // NOLINT(bugprone-easily-swappable-parameters)
                           ) -> std::array<std::int64_t, 2>
{
    // Subdivide into left trapezoidal, right trapezoidal, and center rectangular part
    std::int64_t const left_qmax  = std::min(-r_min - s_max, -r_max - s_min);
    std::int64_t const right_qmin = std::max(std::max(-r_min - s_max, -r_max - s_min), left_qmax + 1);
    std::size_t const  middle_h   = right_qmin - left_qmax - 1;

    // Compute left trapezoid bounds
    std::size_t const left_height = left_qmax - q_min + 1;
    std::size_t const left_top    = r_max - (-q_min - s_max) + 1;
    std::size_t const left_base   = r_max - (-left_qmax - s_max) + 1;
    std::size_t const left_size   = isosceles_trapezoid_size_from_base_and_height(left_base, left_height);

    // Compute right trapezoid bounds
    std::size_t const right_base = (-right_qmin - s_min) - r_min + 1;

    // Compute middle parallelogram
    std::size_t const middle_size = middle_h * left_base;

    // If idx is in the right trapezoid
    if (idx >= left_size + middle_size)
    {
        std::size_t const  cor_idx       = idx - (left_size + middle_size);
        std::size_t const  height        = isosceles_trapezoid_height_from_base_and_size(right_base, cor_idx + 1);
        std::size_t const  row_from_base = height - 1;
        std::int64_t const q             = right_qmin + static_cast<std::int64_t>(row_from_base);
        std::size_t const  tiles_at_base = isosceles_trapezoid_size_from_base_and_height(right_base, row_from_base);
        std::size_t const  r_offset      = cor_idx - tiles_at_base;
        std::int64_t const r             = r_min + static_cast<std::int64_t>(r_offset);
        return {q, r};
    }

    // If idx is in the middle parallelogram
    if (idx >= left_size)
    {
        std::size_t const  cor_idx = idx - left_size;
        std::size_t const  row     = cor_idx / left_base;
        std::size_t const  col     = cor_idx % left_base;
        std::int64_t const q       = left_qmax + 1 + static_cast<std::int64_t>(row);

        bool const left_leaning = -s_max - r_min > -r_max - s_min;
        if (left_leaning)
        {
            std::int64_t const s = s_max - static_cast<std::int64_t>(col);
            return {q, -q - s};
        }
        std::int64_t const r = r_min + static_cast<std::int64_t>(col);
        return {q, r};
    }

    // If idx is in the left trapezoid
    std::size_t const  height       = isosceles_trapezoid_height_from_top_and_size(left_top, idx + 1);
    std::size_t const  row_from_top = height - 1;
    std::int64_t const q            = q_min + static_cast<std::int64_t>(row_from_top);
    std::size_t const  tiles_at_top = isosceles_trapezoid_size_from_top_and_height(left_top, row_from_top);
    std::size_t const  col          = idx - tiles_at_top;
    std::int64_t const s            = s_max - static_cast<std::int64_t>(col);
    return {q, -q - s};
}
} // namespace hex::detail

#endif // HEX_DETAIL_HEXAGON_SIZE_HPP
