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
#include "hex/detail/detail_hexagon_bounds.hpp"

#include <catch2/catch_all.hpp>

using namespace hex::detail;

TEST_CASE("tighten_bounds", "[detail]")
{
    SECTION("bounds already tight")
    {
        int q_min = 0;
        int r_min = 0;
        int s_min = 0;
        int q_max = 0;
        int r_max = 0;
        int s_max = 0;

        CHECK(tighten_bounds(q_min, r_min, s_min, q_max, r_max, s_max));
        CHECK(q_min == 0);
        CHECK(r_min == 0);
        CHECK(s_min == 0);
        CHECK(q_max == 0);
        CHECK(r_min == 0);
        CHECK(s_min == 0);
    }

    SECTION("invalid bounds")
    {
        int q_min = -1;
        int r_min = -1;
        int s_min = -1;
        int q_max = -1;
        int r_max = -1;
        int s_max = -1;

        CHECK(tighten_bounds(q_min, r_min, s_min, q_max, r_max, s_max) == false);
    }

    SECTION("bounds not tight")
    {
        int q_min = 0;
        int r_min = 0;
        int s_min = 0;
        int q_max = -1;
        int r_max = -1;
        int s_max = -1;

        CHECK(tighten_bounds(q_min, r_min, s_min, q_max, r_max, s_max));
        CHECK(q_min == 0);
        CHECK(r_min == 0);
        CHECK(s_min == 0);
        CHECK(q_max == 0);
        CHECK(r_min == 0);
        CHECK(s_min == 0);
    }
}