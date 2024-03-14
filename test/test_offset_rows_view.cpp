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
#include "hex/offset_rows_view.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <ranges>

using namespace hex;

TEST_CASE("offset_rows_view")
{
    using namespace literals;

    SECTION("concepts")
    {
        STATIC_CHECK(std::ranges::random_access_range<offset_rows_view<int>>);
        STATIC_CHECK(std::ranges::sized_range<offset_rows_view<int>>);
    }

    SECTION("q axis")
    {
        SECTION("odd")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::q, offset_parity::odd, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, -2_r},
                vector{-1_q, -1_r},
                vector{-1_q, 0_r},
                vector{-1_q, 1_r},
                vector{-1_q, 2_r},
                // second row
                vector{0_q, -2_r},
                vector{0_q, -1_r},
                vector{0_q, 0_r},
                vector{0_q, 1_r},
                vector{0_q, 2_r},
                // third row
                vector{1_q, -3_r},
                vector{1_q, -2_r},
                vector{1_q, -1_r},
                vector{1_q, 0_r},
                vector{1_q, 1_r},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
        SECTION("even")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::q, offset_parity::even, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, -2_r},
                vector{-1_q, -1_r},
                vector{-1_q, 0_r},
                vector{-1_q, 1_r},
                vector{-1_q, 2_r},
                // second row
                vector{0_q, -3_r},
                vector{0_q, -2_r},
                vector{0_q, -1_r},
                vector{0_q, 0_r},
                vector{0_q, 1_r},
                // third row
                vector{1_q, -3_r},
                vector{1_q, -2_r},
                vector{1_q, -1_r},
                vector{1_q, 0_r},
                vector{1_q, 1_r},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
    }
    SECTION("r axis")
    {
        SECTION("odd")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::r, offset_parity::odd, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, -2_r},
                vector{-2_q, -2_r},
                vector{-3_q, -2_r},
                vector{-4_q, -2_r},
                vector{-5_q, -2_r},
                // second row
                vector{-2_q, -1_r},
                vector{-3_q, -1_r},
                vector{-4_q, -1_r},
                vector{-5_q, -1_r},
                vector{-6_q, -1_r},
                // third row
                vector{-2_q, 0_r},
                vector{-3_q, 0_r},
                vector{-4_q, 0_r},
                vector{-5_q, 0_r},
                vector{-6_q, 0_r},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
        SECTION("even")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::r, offset_parity::even, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, -2_r},
                vector{-2_q, -2_r},
                vector{-3_q, -2_r},
                vector{-4_q, -2_r},
                vector{-5_q, -2_r},
                // second row
                vector{-1_q, -1_r},
                vector{-2_q, -1_r},
                vector{-3_q, -1_r},
                vector{-4_q, -1_r},
                vector{-5_q, -1_r},
                // third row
                vector{-2_q, 0_r},
                vector{-3_q, 0_r},
                vector{-4_q, 0_r},
                vector{-5_q, 0_r},
                vector{-6_q, 0_r},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
    }
    SECTION("s axis")
    {
        SECTION("odd")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::s, offset_parity::odd, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, 3_s},
                vector{0_q, 3_s},
                vector{1_q, 3_s},
                vector{2_q, 3_s},
                vector{3_q, 3_s},
                // second row
                vector{-1_q, 4_s},
                vector{0_q, 4_s},
                vector{1_q, 4_s},
                vector{2_q, 4_s},
                vector{3_q, 4_s},
                // third row
                vector{-2_q, 5_s},
                vector{-1_q, 5_s},
                vector{0_q, 5_s},
                vector{1_q, 5_s},
                vector{2_q, 5_s},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
        SECTION("even")
        {
            constexpr auto view = views::offset_rows(coordinate_axis::s, offset_parity::even, 3, 5, {-1_q, -2_r});
            STATIC_CHECK(view.size() == 5 * 3);

            constexpr auto expected = std::array{
                // first row
                vector{-1_q, 3_s},
                vector{0_q, 3_s},
                vector{1_q, 3_s},
                vector{2_q, 3_s},
                vector{3_q, 3_s},
                // second row
                vector{-2_q, 4_s},
                vector{-1_q, 4_s},
                vector{0_q, 4_s},
                vector{1_q, 4_s},
                vector{2_q, 4_s},
                // third row
                vector{-2_q, 5_s},
                vector{-1_q, 5_s},
                vector{0_q, 5_s},
                vector{1_q, 5_s},
                vector{2_q, 5_s},
            };
            STATIC_CHECK(std::ranges::equal(view, expected));
        }
    }
}