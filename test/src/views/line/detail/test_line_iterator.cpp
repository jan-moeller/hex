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
#include "hex/views/line/detail/detail_line_iterator.hpp"

#include <catch2/catch_all.hpp>

#include <array>
#include <iterator>

#include <cstdlib>

using namespace hex;
using namespace hex::detail;

TEST_CASE("line_iterator", "[detail]")
{
    using namespace literals;

    SECTION("concepts")
    {
        STATIC_CHECK(std::input_iterator<line_iterator<int>>);
        STATIC_CHECK(std::forward_iterator<line_iterator<int>>);
    }

    SECTION("generates the expected positions")
    {
        SECTION("category 1")
        {
            line_iterator<int>          iter({0_q, 0_r}, {4_q, 2_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, 0_r},
                                                 vector{1_q, 1_r},
                                                 vector{2_q, 1_r},
                                                 vector{3_q, 1_r},
                                                 vector{3_q, 2_r},
                                                 vector{4_q, 2_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
        SECTION("category 2")
        {
            line_iterator<int>          iter({0_q, 0_r}, {4_q, -1_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, 0_r},
                                                 vector{2_q, -1_r},
                                                 vector{3_q, -1_r},
                                                 vector{4_q, -1_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
        SECTION("category 3")
        {
            line_iterator<int>          iter({0_q, 0_r}, {3_q, -5_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{1_q, -1_r},
                                                 vector{1_q, -2_r},
                                                 vector{2_q, -3_r},
                                                 vector{2_q, -4_r},
                                                 vector{3_q, -5_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
        SECTION("category 4")
        {
            line_iterator<int>          iter({0_q, 0_r}, {-1_q, -5_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{0_q, -1_r},
                                                 vector{0_q, -2_r},
                                                 vector{-1_q, -2_r},
                                                 vector{-1_q, -3_r},
                                                 vector{-1_q, -4_r},
                                                 vector{-1_q, -5_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
        SECTION("category 5")
        {
            line_iterator<int>          iter({0_q, 0_r}, {-5_q, 2_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{-1_q, 0_r},
                                                 vector{-2_q, 1_r},
                                                 vector{-3_q, 1_r},
                                                 vector{-4_q, 2_r},
                                                 vector{-5_q, 2_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
        SECTION("category 6")
        {
            line_iterator<int>          iter({0_q, 0_r}, {-3_q, 4_r});
            static constexpr std::array expected{vector{0_q, 0_r},
                                                 vector{-1_q, 1_r},
                                                 vector{-1_q, 2_r},
                                                 vector{-2_q, 3_r},
                                                 vector{-3_q, 4_r}};

            for (auto exp = expected.begin(); exp != expected.end(); ++exp, ++iter)
                CHECK(*iter == *exp);
        }
    }
}