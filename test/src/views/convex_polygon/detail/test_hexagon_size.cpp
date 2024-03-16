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
#include "hex/views/convex_polygon/detail/detail_hexagon_size.hpp"
#include "hex/views/convex_polygon/detail/detail_isosceles_trapezoid_size.hpp"

#include <catch2/catch_all.hpp>

#include <array>

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

TEST_CASE("index_to_qr", "[detail]")
{
    using qr = std::array<std::int64_t, 2>;
    SECTION("single element")
    {
        STATIC_CHECK(qr{0, 0} == index_to_qr(0UZ, 0, 0, 0, 0, 0));
        STATIC_CHECK(qr{1, 1} == index_to_qr(0UZ, 1, 1, -2, 1, -2));
        STATIC_CHECK(qr{1, -2} == index_to_qr(0UZ, 1, -2, 1, -2, 1));
        STATIC_CHECK(qr{-2, 1} == index_to_qr(0UZ, -2, 1, 1, 1, 1));
    }
    SECTION("triangle")
    {
        STATIC_CHECK(qr{-1, -1} == index_to_qr(0UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{-1, 0} == index_to_qr(1UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(2UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{-1, 2} == index_to_qr(3UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{0, -1} == index_to_qr(4UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{0, 0} == index_to_qr(5UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{0, 1} == index_to_qr(6UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{1, -1} == index_to_qr(7UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{1, 0} == index_to_qr(8UZ, -1, -1, -1, 2, 2));
        STATIC_CHECK(qr{2, -1} == index_to_qr(9UZ, -1, -1, -1, 2, 2));

        STATIC_CHECK(qr{-2, 1} == index_to_qr(0UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{-1, 0} == index_to_qr(1UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(2UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{0, -1} == index_to_qr(3UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(4UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{0, 1} == index_to_qr(5UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{1, -2} == index_to_qr(6UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{1, -1} == index_to_qr(7UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{1, 0} == index_to_qr(8UZ, -2, -2, -2, 1, 1));
        STATIC_CHECK(qr{1, 1} == index_to_qr(9UZ, -2, -2, -2, 1, 1));
    }
    SECTION("isosceles trapezoid")
    {
        STATIC_CHECK(qr{-1, 0} == index_to_qr(0UZ, -1, 0, -1, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(1UZ, -1, 0, -1, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(2UZ, -1, 0, -1, 1, 1));
        STATIC_CHECK(qr{0, 1} == index_to_qr(3UZ, -1, 0, -1, 1, 1));
        STATIC_CHECK(qr{1, 0} == index_to_qr(4UZ, -1, 0, -1, 1, 1));
    }
    SECTION("parallelogram")
    {
        STATIC_CHECK(qr{-1, 0} == index_to_qr(0UZ, -1, -1, 0, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(1UZ, -1, -1, 0, 1, 1));
        STATIC_CHECK(qr{0, -1} == index_to_qr(2UZ, -1, -1, 0, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(3UZ, -1, -1, 0, 1, 1));

        STATIC_CHECK(qr{-1, -1} == index_to_qr(0, -1, -1, -1, 0, 2));
        STATIC_CHECK(qr{-1, 0} == index_to_qr(1, -1, -1, -1, 0, 2));
        STATIC_CHECK(qr{0, -1} == index_to_qr(2, -1, -1, -1, 0, 2));
        STATIC_CHECK(qr{0, 0} == index_to_qr(3, -1, -1, -1, 0, 2));
        STATIC_CHECK(qr{1, -1} == index_to_qr(4, -1, -1, -1, 0, 2));
        STATIC_CHECK(qr{1, 0} == index_to_qr(5, -1, -1, -1, 0, 2));

        STATIC_CHECK(qr{-1, 0} == index_to_qr(0, -1, -2, 0, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(1, -1, -2, 0, 1, 1));
        STATIC_CHECK(qr{0, -1} == index_to_qr(2, -1, -2, 0, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(3, -1, -2, 0, 1, 1));
        STATIC_CHECK(qr{1, -2} == index_to_qr(4, -1, -2, 0, 1, 1));
        STATIC_CHECK(qr{1, -1} == index_to_qr(5, -1, -2, 0, 1, 1));
    }
    SECTION("pentagon")
    {
        STATIC_CHECK(qr{-1, 0} == index_to_qr(0UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(1UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{0, -1} == index_to_qr(2UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(3UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{0, 1} == index_to_qr(4UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{1, -2} == index_to_qr(5UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{1, -1} == index_to_qr(6UZ, -1, -2, -1, 1, 1));
        STATIC_CHECK(qr{1, 0} == index_to_qr(7UZ, -1, -2, -1, 1, 1));
    }
    SECTION("hexagon")
    {
        STATIC_CHECK(qr{-1, 0} == index_to_qr(0UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{-1, 1} == index_to_qr(1UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{0, -1} == index_to_qr(2UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{0, 0} == index_to_qr(3UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{0, 1} == index_to_qr(4UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{1, -1} == index_to_qr(5UZ, -1, -1, -1, 1, 1));
        STATIC_CHECK(qr{1, 0} == index_to_qr(6UZ, -1, -1, -1, 1, 1));
    }
    SECTION("max size")
    {
        static constexpr std::int32_t radius = isosceles_trapezoid_max_base / 2;
        static constexpr auto         size   = regular_hexagon_size(radius);
        STATIC_CHECK(qr{-radius, 0} == index_to_qr(0, -radius, -radius, -radius, radius, radius));
        STATIC_CHECK(qr{-radius, 1} == index_to_qr(1, -radius, -radius, -radius, radius, radius));
        STATIC_CHECK(qr{radius, 0} == index_to_qr(size - 1, -radius, -radius, -radius, radius, radius));

        STATIC_CHECK(qr{0, radius} == index_to_qr(0, 0, 0, -radius * 3LL, 2LL * radius, -radius));
        STATIC_CHECK(qr{0, radius + 1} == index_to_qr(1, 0, 0, -radius * 3LL, 2LL * radius, -radius));
        STATIC_CHECK(qr{2 * radius, radius} == index_to_qr(size - 1, 0, 0, -radius * 3LL, 2LL * radius, -radius));
    }
}

// TEST_CASE("foo")
// {
//     constexpr auto diag_len = [](int m, int n, int k)
//     {
//         return std::min({m, n, m - k, n + k});
//     };
//     CHECK(diag_len(3, 3, 0) == 3);
//     CHECK(diag_len(3, 3, 1) == 2);
//     CHECK(diag_len(3, 3, -1) == 2);
//     CHECK(diag_len(3, 2, 0) == 2);
//     CHECK(diag_len(3, 2, 1) == 2);
//     CHECK(diag_len(3, 2, -1) == 1);
//
//     constexpr auto rev_diag_len = [diag_len](int m, int n, int k)
//     {
//         return diag_len(m, n, k - n + 1);
//     };
//     CHECK(rev_diag_len(3, 3, 0) == 1);
//     CHECK(rev_diag_len(3, 3, 1) == 2);
//     CHECK(rev_diag_len(3, 3, 2) == 3);
//     CHECK(rev_diag_len(3, 3, 3) == 2);
//     CHECK(rev_diag_len(3, 3, 4) == 1);
//     CHECK(rev_diag_len(3, 3, 5) == 0);
//     CHECK(rev_diag_len(3, 2, 0) == 1);
//     CHECK(rev_diag_len(3, 2, 1) == 2);
//     CHECK(rev_diag_len(3, 2, 2) == 2);
//     CHECK(rev_diag_len(3, 2, 3) == 1);
//     CHECK(rev_diag_len(3, 2, 4) == 0);
//
//     constexpr auto size = [rev_diag_len](int q_min, int r_min, int s_min, int q_max, int r_max, int s_max)
//     {
//         auto const z = -q_min - r_min - s_min;
//         auto const m = q_max - q_min + 1;
//         auto const n = r_max - r_min + 1;
//         auto const o = s_max - s_min + 1;
//
//         auto const hi = rev_diag_len(m, n, z);
//         auto const k  = (z + 1) - hi;
//         auto const lo = hi - std::min(o - k, z + 1 - k);
//
//         auto const res  = (-lo * lo + lo + hi * hi + hi) / 2;
//         auto const res2 = hi * k;
//         return res + res2;
//     };
//
//     CHECK(size(-1, 0, -1, 1, 1, 1) == 5);
//     CHECK(size(0, 0, 0, 0, 0, 0) == 1);
//     CHECK(size(-2, -2, -2, 1, 1, 1) == 10);
//     CHECK(size(-1, -1, 0, 0, 1, 1) == 4);
//     CHECK(size(-1, -1, -1, 1, 0, 2) == 6);
//     CHECK(size(-1, -2, 0, 1, 1, 1) == 6);
//     CHECK(size(-2, -2, -2, 2, 2, 2) == 19);
// }