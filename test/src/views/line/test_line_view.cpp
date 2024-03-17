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
#include "hex/vector/vector.hpp"
#include "hex/views/line/line_view.hpp"

#include <catch2/catch_all.hpp>

#include <array>
#include <iterator>
#include <ranges>

#include <cstddef>

using namespace hex;

TEST_CASE("line_view")
{
    using namespace literals;

    SECTION("concepts")
    {
        STATIC_CHECK(std::ranges::sized_range<line_view<int>>);
        STATIC_CHECK(std::ranges::common_range<line_view<int>>);
        STATIC_CHECK(std::ranges::forward_range<line_view<int>>);
        STATIC_CHECK(std::ranges::borrowed_range<line_view<int>>);
        STATIC_CHECK(std::ranges::constant_range<line_view<int>>);
    }

    SECTION("distance & size")
    {
        vector<int> const from = GENERATE(vector{0_q, 0_r}, vector{-1_q, 0_r}, vector{1_q, 0_r}, vector{42_q, 999_r});
        vector<int> const to   = GENERATE(vector{0_q, 0_r}, vector{-1_q, 0_r}, vector{1_q, 0_r}, vector{42_q, 999_r});

        CHECK((std::size_t)std::ranges::distance(views::line(from, to)) == views::line(from, to).size());
        CHECK(std::ranges::distance(views::line(from, to)) == distance(from, to));
    }

    SECTION("content")
    {
        SECTION("category 1")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, 0_r},
                                                 vector{1_q, 1_r},
                                                 vector{2_q, 1_r},
                                                 vector{3_q, 1_r},
                                                 vector{3_q, 2_r},
                                                 vector{4_q, 2_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
        SECTION("category 2")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, 0_r},
                                                 vector{2_q, -1_r},
                                                 vector{3_q, -1_r},
                                                 vector{4_q, -1_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
        SECTION("category 3")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, -1_r},
                                                 vector{1_q, -2_r},
                                                 vector{2_q, -3_r},
                                                 vector{2_q, -4_r},
                                                 vector{3_q, -5_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
        SECTION("category 4")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{0_q, -1_r},
                                                 vector{0_q, -2_r},
                                                 vector{-1_q, -2_r},
                                                 vector{-1_q, -3_r},
                                                 vector{-1_q, -4_r},
                                                 vector{-1_q, -5_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
        SECTION("category 5")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{-1_q, 0_r},
                                                 vector{-2_q, 1_r},
                                                 vector{-3_q, 1_r},
                                                 vector{-4_q, 2_r},
                                                 vector{-5_q, 2_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
        SECTION("category 6")
        {
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{-1_q, 1_r},
                                                 vector{-1_q, 2_r},
                                                 vector{-2_q, 3_r},
                                                 vector{-3_q, 4_r}};

            for (auto&& [exp, val] : std::views::zip(expected, views::line(expected.front(), expected.back())))
                CHECK(exp == val);
        }
    }
}