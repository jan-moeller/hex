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
#include "hex/detail/detail_bounded_polygon_iterator.hpp"
#include "hex/vector.hpp"

#include <catch2/catch_all.hpp>

#include <iterator>
#include <vector>

using namespace hex;
using namespace hex::detail;

TEST_CASE("bounded_polygon_iterator", "[detail]")
{
    using namespace hex::literals;

    SECTION("concept checks")
    {
        STATIC_CHECK(std::weakly_incrementable<bounded_polygon_iterator<int>>);
        STATIC_CHECK(std::input_or_output_iterator<bounded_polygon_iterator<int>>);
        STATIC_CHECK(std::indirectly_readable<bounded_polygon_iterator<int>>);
        STATIC_CHECK(std::input_iterator<bounded_polygon_iterator<int>>);
        STATIC_CHECK(std::forward_iterator<bounded_polygon_iterator<int>>);
        STATIC_CHECK(std::bidirectional_iterator<bounded_polygon_iterator<int>>);
    }

    SECTION("default constructed must compare equal")
    {
        STATIC_CHECK(bounded_polygon_iterator<int>() == bounded_polygon_iterator<int>());
    }
    SECTION("++iter")
    {
        bounded_polygon_iterator<int> iter(-1_r, -1_s, 2_r, 1_s, vector{-2_q, 1_r});
        ++iter;
        CHECK(*iter == vector{-2_q, 2_r});
        ++iter;
        CHECK(*iter == vector{-1_q, 0_r});
    }
    SECTION("iter++")
    {
        bounded_polygon_iterator<int> iter(-1_r, -1_s, 2_r, 1_s, vector{-2_q, 1_r});
        CHECK(*(iter++) == vector{-2_q, 1_r});
        CHECK(*iter == vector{-2_q, 2_r});
    }
    SECTION("--iter")
    {
        bounded_polygon_iterator<int> iter(-1_r, -1_s, 2_r, 1_s, vector{-1_q, 1_r});
        --iter;
        CHECK(*iter == vector{-1_q, 0_r});
        --iter;
        CHECK(*iter == vector{-2_q, 2_r});
    }
    SECTION("iter--")
    {
        bounded_polygon_iterator<int> iter(-1_r, -1_s, 2_r, 1_s, vector{-1_q, 1_r});
        CHECK(*(iter--) == vector{-1_q, 1_r});
        CHECK(*iter == vector{-1_q, 0_r});
    }

    SECTION("ordering")
    {
        constexpr bounded_polygon_iterator<int> a(-1_r, -1_s, 2_r, 1_s, vector{-2_q, 1_r});
        constexpr bounded_polygon_iterator<int> b(-1_r, -1_s, 2_r, 1_s, vector{-1_q, 1_r});
        constexpr bounded_polygon_iterator<int> c(-1_r, -1_s, 2_r, 1_s, vector{-1_q, 2_r});
        STATIC_CHECK(a < b);
        STATIC_CHECK(b < c);
        STATIC_CHECK(b == b);
    }

    SECTION("iterates the expected elements")
    {
        SECTION("-q triangle")
        {
            auto const begin = bounded_polygon_iterator<int>(-2_r, -1_s, 0_r, 1_s, vector{-1_q, 0_r});
            auto const end   = ++bounded_polygon_iterator<int>(-2_r, -1_s, 0_r, 1_s, vector{1_q, 0_r});

            CHECK(std::ranges::distance(begin, end) == 6);

            std::vector<hex::vector<int>> const expected{{-1_q, 0_r},
                                                         {0_q, -1_r},
                                                         {0_q, 0_r},
                                                         {1_q, -2_r},
                                                         {1_q, -1_r},
                                                         {1_q, 0_r}};
            std::vector<hex::vector<int>> const elements(begin, end);
            CHECK(elements == expected);
        }
        SECTION("+q triangle")
        {
            auto const begin = bounded_polygon_iterator<int>(-1_r, 0_s, 1_r, 2_s, vector{-1_q, -1_r});
            auto const end   = ++bounded_polygon_iterator<int>(-1_r, 0_s, 1_r, 2_s, vector{1_q, -1_r});

            CHECK(std::ranges::distance(begin, end) == 6);

            std::vector<hex::vector<int>> const expected{{-1_q, -1_r},
                                                         {-1_q, 0_r},
                                                         {-1_q, 1_r},
                                                         {0_q, -1_r},
                                                         {0_q, 0_r},
                                                         {1_q, -1_r}};
            std::vector<hex::vector<int>> const elements(begin, end);
            CHECK(elements == expected);
        }
        SECTION("quadrangle")
        {
            auto const begin = bounded_polygon_iterator<int>(-1_r, -1_s, 0_r, 1_s, vector{-1_q, 0_r});
            auto const end   = ++bounded_polygon_iterator<int>(-1_r, -1_s, 0_r, 1_s, vector{1_q, 0_r});

            CHECK(std::ranges::distance(begin, end) == 5);

            std::vector<hex::vector<int>> const expected{{-1_q, 0_r},
                                                         {0_q, -1_r},
                                                         {0_q, 0_r},
                                                         {1_q, -1_r},
                                                         {1_q, 0_r}};
            std::vector<hex::vector<int>> const elements(begin, end);
            CHECK(elements == expected);
        }
    }
}