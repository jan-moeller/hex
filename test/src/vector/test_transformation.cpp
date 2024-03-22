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
#include "hex/vector/rotation_steps.hpp"
#include "hex/vector/transformation.hpp"
#include "hex/vector/vector.hpp"

#include <catch2/catch_all.hpp>

#include <numbers>

using namespace hex;

TEST_CASE("transformation")
{
    using namespace literals;

    SECTION("identity")
    {
        STATIC_CHECK(apply(transformation{}, vector{0_q, 0_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{}, vector{1_q, 2_r}) == vector{1_q, 2_r});
    }
    SECTION("translation")
    {
        STATIC_CHECK(apply(transformation{translation{vector{0_q, 0_r}}}, vector{0_q, 0_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{translation{vector{1_q, 2_r}}}, vector{0_q, 0_r}) == vector{1_q, 2_r});
        STATIC_CHECK(apply(transformation{translation{vector{1_q, 2_r}}}, vector{-1_q, -2_r}) == vector{0_q, 0_r});
    }
    SECTION("60° rotation")
    {
        STATIC_CHECK(apply(transformation{rot_300_ccw}, vector{1_q, 2_r}) == vector{-2_q, 3_r});
        STATIC_CHECK(apply(transformation{rot_240_ccw}, vector{1_q, 2_r}) == vector{-3_q, 1_r});
        STATIC_CHECK(apply(transformation{rot_180_ccw}, vector{1_q, 2_r}) == vector{-1_q, -2_r});
        STATIC_CHECK(apply(transformation{rot_120_ccw}, vector{1_q, 2_r}) == vector{2_q, -3_r});
        STATIC_CHECK(apply(transformation{rot_60_ccw}, vector{1_q, 2_r}) == vector{3_q, -1_r});
        STATIC_CHECK(apply(transformation{rot_0}, vector{1_q, 2_r}) == vector{1_q, 2_r});
        STATIC_CHECK(apply(transformation{rot_60_cw}, vector{1_q, 2_r}) == vector{-2_q, 3_r});
        STATIC_CHECK(apply(transformation{rot_120_cw}, vector{1_q, 2_r}) == vector{-3_q, 1_r});
        STATIC_CHECK(apply(transformation{rot_180_cw}, vector{1_q, 2_r}) == vector{-1_q, -2_r});
        STATIC_CHECK(apply(transformation{rot_240_cw}, vector{1_q, 2_r}) == vector{2_q, -3_r});
        STATIC_CHECK(apply(transformation{rot_300_cw}, vector{1_q, 2_r}) == vector{3_q, -1_r});

        STATIC_CHECK(apply(transformation{rot_300_cw}, vector{0_q, 0_r}) == vector{0_q, 0_r});
    }
    SECTION("rotation")
    {
        constexpr auto deg_60 = std::numbers::pi_v<double> / 3.;
        STATIC_CHECK(apply(transformation{rotation{deg_60}}, vector{0_q, 0_r}) == vector{0._q, 0._r});
        CHECK_THAT(apply(transformation{rotation{deg_60}}, vector{1_q, 2_r}).q().value(),
                   Catch::Matchers::WithinRel(-2.));
        CHECK_THAT(apply(transformation{rotation{deg_60}}, vector{1_q, 2_r}).r().value(),
                   Catch::Matchers::WithinRel(3.));
        CHECK_THAT(apply(transformation{rotation{-deg_60}}, vector{1_q, 2_r}).q().value(),
                   Catch::Matchers::WithinRel(3.));
        CHECK_THAT(apply(transformation{rotation{-deg_60}}, vector{1_q, 2_r}).r().value(),
                   Catch::Matchers::WithinRel(-1.));
    }
    SECTION("reflection")
    {
        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::q}}, vector{0_q, 0_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::r}}, vector{0_q, 0_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::s}}, vector{0_q, 0_r}) == vector{0_q, 0_r});

        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::q}}, vector{1_q, 2_r}) == vector{-1_q, 3_r});
        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::r}}, vector{1_q, 2_r}) == vector{3_q, -2_r});
        STATIC_CHECK(apply(transformation{reflection{coordinate_axis::s}}, vector{1_q, 2_r}) == vector{-2_q, -1_r});
    }
    SECTION("scaling")
    {
        STATIC_CHECK(apply(transformation{scaling{0}}, vector{1_q, 2_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{scaling{1}}, vector{0_q, 0_r}) == vector{0_q, 0_r});
        STATIC_CHECK(apply(transformation{scaling{1}}, vector{1_q, 2_r}) == vector{1_q, 2_r});
        STATIC_CHECK(apply(transformation{scaling{2}}, vector{1_q, 2_r}) == vector{2_q, 4_r});
        STATIC_CHECK(apply(transformation{scaling{-2}}, vector{1_q, 2_r}) == vector{-2_q, -4_r});

        STATIC_CHECK(apply(transformation{scaling{2._q}}, vector{1_q, 2_r}) == vector{1._q, 4.5_r});
        STATIC_CHECK(apply(transformation{scaling{2._r}}, vector{1_q, 2_r}) == vector{3._q, 2._r});
        STATIC_CHECK(apply(transformation{scaling{2._s}}, vector{1_q, 2_r}) == vector{0.5_q, 2.5_r});
    }
    SECTION("shearing")
    {
        STATIC_CHECK(apply(transformation{shearing{0_q}}, vector{1_q, 2_r}) == vector{1_q, 2_r});
        STATIC_CHECK(apply(transformation{shearing{0_r}}, vector{1_q, 2_r}) == vector{1_q, 2_r});
        STATIC_CHECK(apply(transformation{shearing{0_s}}, vector{1_q, 2_r}) == vector{1_q, 2_r});

        STATIC_CHECK(apply(transformation{shearing{1_q}}, vector{1_q, 2_r}) == vector{1_q, 3_r});
        STATIC_CHECK(apply(transformation{shearing{1_r}}, vector{1_q, 2_r}) == vector{-1_q, 2_r});
        STATIC_CHECK(apply(transformation{shearing{1_s}}, vector{1_q, 2_r}) == vector{-2_q, 5_r});

        STATIC_CHECK(apply(transformation{shearing{-1_q}}, vector{1_q, 2_r}) == vector{1_q, 1_r});
        STATIC_CHECK(apply(transformation{shearing{-1_r}}, vector{1_q, 2_r}) == vector{3_q, 2_r});
        STATIC_CHECK(apply(transformation{shearing{-1_s}}, vector{1_q, 2_r}) == vector{4_q, -1_r});
    }
    SECTION("combine")
    {
        constexpr transformation translate = translation{vector{1_q, -1_r}};
        constexpr transformation rot60     = rot_60_cw;
        constexpr transformation reflq     = reflection{coordinate_axis::q};
        STATIC_CHECK(apply(combine(translate, rot60), vector{1_q, 2_r}) == vector{-1_q, 3_r});
        STATIC_CHECK(apply(combine(rot60, translate), vector{1_q, 2_r}) == vector{-1_q, 2_r});
        STATIC_CHECK(apply(combine(rot60, translate, reflq, rot60), vector{1_q, 2_r}) == vector{-1_q, 2_r});
    }
}