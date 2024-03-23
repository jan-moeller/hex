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
#include "hex/vector/coordinate_axis.hpp"
#include "hex/vector/rotation.hpp"
#include "hex/vector/rotation_steps.hpp"
#include "hex/vector/scaling.hpp"
#include "hex/vector/vector.hpp"
#include "hex/views/transform/transform_view.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <array>
#include <numbers>
#include <ranges>

using namespace hex;

TEST_CASE("transform_view")
{
    using namespace literals;

    constexpr std::array<vector<int>, 3> line = {vector{0_q, 0_r}, vector{1_q, 0_r}, vector{1_q, 1_r}};

    SECTION("reflection")
    {
        std::array<vector<int>, 3> results;

        std::ranges::copy(line | views::transform(reflection{coordinate_axis::q}), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{-1_q, 1_r});
        CHECK(results[2] == vector{-1_q, 2_r});

        std::ranges::copy(line | views::transform(reflection{coordinate_axis::r}), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{1_q, 0_r});
        CHECK(results[2] == vector{2_q, -1_r});

        std::ranges::copy(line | views::transform(reflection{coordinate_axis::s}), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{0_q, -1_r});
        CHECK(results[2] == vector{-1_q, -1_r});

        CHECK(std::ranges::equal(results, views::transform(line, reflection{coordinate_axis::s})));
    }

    SECTION("rotation")
    {
        std::array<vector<double>, 3> results;
        constexpr auto                deg_60 = std::numbers::pi_v<double> / 3.;

        std::ranges::copy(line | views::transform(rotation{-deg_60}), results.begin());
        CHECK_THAT(results[0].q().value(), Catch::Matchers::WithinRel(0.));
        CHECK_THAT(results[0].r().value(), Catch::Matchers::WithinRel(0.));
        CHECK_THAT(results[1].q().value(), Catch::Matchers::WithinRel(1.));
        CHECK_THAT(results[1].r().value(), Catch::Matchers::WithinRel(-1.));
        CHECK_THAT(results[2].q().value(), Catch::Matchers::WithinRel(2.));
        CHECK_THAT(results[2].r().value(), Catch::Matchers::WithinRel(-1.));

        CHECK(std::ranges::equal(results, views::transform(line, rotation{-deg_60})));
    }

    SECTION("60° rotation steps")
    {
        std::array<vector<int>, 3> results;

        std::ranges::copy(line | views::transform(rot_120_cw), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{-1_q, 1_r});
        CHECK(results[2] == vector{-2_q, 1_r});

        CHECK(std::ranges::equal(results, views::transform(line, rot_120_cw)));
    }

    SECTION("scaling")
    {
        std::array<vector<int>, 3> results;

        std::ranges::copy(line | views::transform(scaling{2}), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{2_q, 0_r});
        CHECK(results[2] == vector{2_q, 2_r});

        CHECK(std::ranges::equal(results, views::transform(line, scaling{2})));
    }

    SECTION("shearing")
    {
        std::array<vector<int>, 3> results;

        std::ranges::copy(line | views::transform(shearing{1_q}), results.begin());
        CHECK(results[0] == vector{0_q, 0_r});
        CHECK(results[1] == vector{1_q, 1_r});
        CHECK(results[2] == vector{1_q, 2_r});

        CHECK(std::ranges::equal(results, views::transform(line, shearing{1_q})));
    }

    SECTION("translation")
    {
        std::array<vector<int>, 3> results;

        std::ranges::copy(line | views::transform(translation{vector{1_q, 2_r}}), results.begin());
        CHECK(results[0] == vector{1_q, 2_r});
        CHECK(results[1] == vector{2_q, 2_r});
        CHECK(results[2] == vector{2_q, 3_r});

        CHECK(std::ranges::equal(results, views::transform(line, translation{vector{1_q, 2_r}})));
    }
}