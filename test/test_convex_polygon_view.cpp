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
#include "hex/convex_polygon_parameters.hpp"
#include "hex/convex_polygon_view.hpp"
#include "hex/vector.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>

using namespace hex;

TEST_CASE("convex_polygon_view")
{
    using namespace literals;

    constexpr convex_polygon_parameters<int> params(-2_q, -1_r, -1_s, 1_q, 2_r, 1_s);

    SECTION("bidirectional range")
    {
        STATIC_CHECK(std::ranges::bidirectional_range<convex_polygon_view<int>>);
        STATIC_CHECK(std::views::reverse(views::convex_polygon(params)).size() == 10);
    }

    SECTION("sized range")
    {
        STATIC_CHECK(std::ranges::sized_range<convex_polygon_view<int>>);
        STATIC_CHECK(std::ranges::size(views::convex_polygon(params)) == 10);
    }

    SECTION("common range")
    {
        STATIC_CHECK(std::ranges::common_range<convex_polygon_view<int>>);
    }

    SECTION("borrowed range")
    {
        STATIC_CHECK(std::ranges::borrowed_range<convex_polygon_view<int>>);
        STATIC_CHECK(*[&]{return views::convex_polygon(params).begin();}() == vector{-2_q, 1_r});
    }

    SECTION("constant range")
    {
        STATIC_CHECK(std::ranges::constant_range<convex_polygon_view<int>>);
    }

    SECTION("generates the expected data")
    {
        STATIC_CHECK(std::ranges::all_of(views::convex_polygon(params),
                                         [&params](vector<int> const& n) { return params.contains(n); }));
        STATIC_CHECK(std::ranges::all_of(views::convex_polygon(params) | std::views::adjacent<2>,
                                         [](auto const& n) { return std::get<0>(n) < std::get<1>(n); }));
    }

    SECTION("filter usecase")
    {
        constexpr convex_polygon_parameters<int> excluded = make_regular_triangle_parameters(1_q, 0_r, 0_s);
        STATIC_CHECK(
            std::ranges::distance(views::convex_polygon(params)
                                  | std::views::filter([&excluded](vector<int> v) { return !excluded.contains(v); }))
            == 7);
    }
}