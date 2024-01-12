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
#include "hex/neighbors_view.hpp"

#include <catch2/catch_all.hpp>

using namespace hex;

TEST_CASE("neighbors_view")
{
    using namespace literals;

    SECTION("neighbors()")
    {
        STATIC_CHECK(std::ranges::size(views::neighbors()) == 6);

        constexpr vector center = {42_q, -2_r};
        STATIC_CHECK(std::ranges::all_of(views::neighbors(center),
                                         [&center](vector<int> const& n) { return adjacent(n, center); }));

        STATIC_CHECK(std::ranges::random_access_range<decltype(views::neighbors())>);
    }
    SECTION("diagonals()")
    {
        STATIC_CHECK(std::ranges::size(views::diagonals()) == 6);

        constexpr vector center = {42_q, -2_r};
        STATIC_CHECK(std::ranges::all_of(views::diagonals(center),
                                         [&center](vector<int> const& n) { return distance(n, center) == 2; }));

        STATIC_CHECK(std::ranges::random_access_range<decltype(views::diagonals())>);
    }
}