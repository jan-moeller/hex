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
#include "hex/detail/detail_sqrt.hpp"

#include <catch2/catch_all.hpp>

#include <limits>

#include <cstdint>

using namespace hex::detail;

TEST_CASE("floor_sqrt", "[detail]")
{
    STATIC_CHECK(floor_sqrt(0U) == 0U);
    STATIC_CHECK(floor_sqrt(1U) == 1U);
    STATIC_CHECK(floor_sqrt(2U) == 1U);
    STATIC_CHECK(floor_sqrt(3U) == 1U);
    STATIC_CHECK(floor_sqrt(3U) == 1U);
    STATIC_CHECK(floor_sqrt(4U) == 2U);
    STATIC_CHECK(floor_sqrt(9U) == 3U);
    STATIC_CHECK(floor_sqrt(std::numeric_limits<std::uint16_t>::max()) == std::numeric_limits<std::uint8_t>::max());
    STATIC_CHECK(floor_sqrt(std::numeric_limits<std::uint32_t>::max()) == std::numeric_limits<std::uint16_t>::max());
    STATIC_CHECK(floor_sqrt(std::numeric_limits<std::uint64_t>::max()) == std::numeric_limits<std::uint32_t>::max());
}

TEST_CASE("ceil_sqrt", "[detail]")
{
    STATIC_CHECK(ceil_sqrt(0U) == 0U);
    STATIC_CHECK(ceil_sqrt(1U) == 1U);
    STATIC_CHECK(ceil_sqrt(2U) == 2U);
    STATIC_CHECK(ceil_sqrt(3U) == 2U);
    STATIC_CHECK(ceil_sqrt(3U) == 2U);
    STATIC_CHECK(ceil_sqrt(4U) == 2U);
    STATIC_CHECK(ceil_sqrt(9U) == 3U);
    STATIC_CHECK(ceil_sqrt(std::numeric_limits<std::uint16_t>::max()) - 1 == std::numeric_limits<std::uint8_t>::max());
    STATIC_CHECK(ceil_sqrt(std::numeric_limits<std::uint32_t>::max()) - 1 == std::numeric_limits<std::uint16_t>::max());
    STATIC_CHECK(ceil_sqrt(std::numeric_limits<std::uint64_t>::max()) - 1 == std::numeric_limits<std::uint32_t>::max());
}