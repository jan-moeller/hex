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
#include "hex/convex_polygon_view.hpp"
#include "hex/grid.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>

#include <cstddef>

using namespace hex;
using namespace hex::literals;

TEST_CASE("grid")
{
    using namespace literals;

    constexpr convex_polygon_view<int> triangle_shape{make_regular_triangle_parameters(1_q, 0_r, 1_s)};
    constexpr convex_polygon_view<int> quadrangle_shape{convex_polygon_parameters{-1_q, -1_r, -1_s, 1_q, 0_r, 1_s}};

    using convex_grid = grid<int, convex_polygon_view<int>>;

    SECTION("concepts")
    {
        STATIC_CHECK(std::ranges::sized_range<convex_grid>);
        STATIC_CHECK(std::ranges::bidirectional_range<convex_grid>);
        STATIC_CHECK(std::ranges::common_range<convex_grid>);
    }

    SECTION("constructor with shape")
    {
        convex_grid const triangular_grid(triangle_shape);
        CHECK(triangular_grid.size() == 6);
        CHECK(static_cast<std::size_t>(std::ranges::distance(triangular_grid)) == triangular_grid.size());
        CHECK(std::ranges::all_of(triangular_grid, [](auto&& e) { return e.second == 0; }));

        convex_grid const quadrangular_grid(quadrangle_shape);
        CHECK(quadrangular_grid.size() == 5);
        CHECK(static_cast<std::size_t>(std::ranges::distance(quadrangular_grid)) == quadrangular_grid.size());
        CHECK(std::ranges::all_of(quadrangular_grid, [](auto&& e) { return e.second == 0; }));
    }
    SECTION("constructor with allocator")
    {
        convex_grid const triangular_grid(triangle_shape, std::allocator<int>());
        CHECK(triangular_grid.size() == 6);
        CHECK(std::ranges::all_of(triangular_grid, [](auto&& e) { return e.second == 0; }));

        convex_grid const quadrangular_grid(quadrangle_shape, std::allocator<int>());
        CHECK(quadrangular_grid.size() == 5);
        CHECK(std::ranges::all_of(quadrangular_grid, [](auto&& e) { return e.second == 0; }));
    }
    SECTION("constructor with initializer list")
    {
        convex_grid const triangular_grid({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape, std::allocator<int>());
        CHECK(triangular_grid.size() == 6);

        convex_grid const quadrangular_grid({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}},
                                            quadrangle_shape,
                                            std::allocator<int>());
        CHECK(quadrangular_grid.size() == 5);
    }

    SECTION("operator[]")
    {
        convex_grid       g1({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);
        convex_grid const g2({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);

        CHECK(g1[{-1_q, 0_r}] == 1);
        CHECK(g2[{0_q, 0_r}] == 42);

        g1[{-1_q, 0_r}] = 2;
        CHECK(g1[{-1_q, 0_r}] == 2);
    }
    SECTION("at()")
    {
        convex_grid       g1({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);
        convex_grid const g2({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);

        CHECK(g1.at({-1_q, 0_r}) == 1);
        CHECK(g2.at({0_q, 0_r}) == 42);
        CHECK_THROWS_AS(g1.at({-5_q, 5_r}), std::out_of_range);
        CHECK_THROWS_AS(g2.at({-5_q, 5_r}), std::out_of_range);

        g1.at({-1_q, 0_r}) = 2;
        CHECK(g1.at({-1_q, 0_r}) == 2);
    }

    SECTION("iteration")
    {
        convex_grid triangular_grid(triangle_shape);
        convex_grid quadrangular_grid(quadrangle_shape);

        CHECK(static_cast<std::size_t>(std::ranges::distance(triangular_grid)) == triangular_grid.size());
        CHECK(static_cast<std::size_t>(std::ranges::distance(quadrangular_grid)) == quadrangular_grid.size());

        int i = 0;
        for (auto& v : triangular_grid | std::views::values)
            v = i++;
        i = 0;
        for (auto& v : quadrangular_grid | std::views::values)
            v = i++;

        i = 0;
        for (auto [a, b] : std::views::zip(std::as_const(triangular_grid), triangle_shape))
        {
            CAPTURE(i);
            CHECK(a.first == b);
            CHECK(a.second == i++);
        }
        CHECK(static_cast<std::size_t>(i) == triangular_grid.size());

        i = static_cast<int>(quadrangular_grid.size());
        for (auto [a, b] : std::views::zip(std::as_const(quadrangular_grid) | std::views::reverse,
                                           quadrangle_shape | std::views::reverse))
        {
            CAPTURE(i);
            CHECK(a.first == b);
            CHECK(a.second == --i);
        }
        CHECK(i == 0);
    }
    SECTION("default-constructed iterator equals end()")
    {
        convex_grid const triangular_grid(triangle_shape);
        CHECK(triangular_grid.end() == convex_grid::const_iterator{});
    }

    SECTION("swap")
    {
        convex_grid triangular_grid({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);
        convex_grid quadrangular_grid(quadrangle_shape);

        triangular_grid.swap(quadrangular_grid);

        CHECK(triangular_grid.size() == quadrangle_shape.size());
        CHECK(quadrangular_grid.size() == triangle_shape.size());
        CHECK(quadrangular_grid[{-1_q, 0_r}] == 1);
        CHECK(triangular_grid[{-1_q, 0_r}] == 0);
    }

    SECTION("find")
    {
        convex_grid const triangular_grid({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);

        auto iter = triangular_grid.find({-1_q, 0_r});
        CHECK((*iter).first == vector{-1_q, 0_r});
        CHECK((*iter).second == 1);
    }

    SECTION("contains")
    {
        convex_grid const triangular_grid({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);
        CHECK(triangular_grid.contains({-1_q, 0_r}));
        CHECK(!triangular_grid.contains({-2_q, 0_r}));
    }

    SECTION("operator==")
    {
        convex_grid g1({{{-1_q, 0_r}, 1}, {{0_q, 0_r}, 42}}, triangle_shape);
        convex_grid g2(triangle_shape);
        convex_grid g3(quadrangle_shape);

        CHECK(g1 != g2);
        CHECK(g1 != g3);
        CHECK(g2 != g3);
        CHECK(g1 == g1);
        CHECK(g2 == g2);
        CHECK(g3 == g3);
    }
}