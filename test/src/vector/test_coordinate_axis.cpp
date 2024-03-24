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

#include <catch2/catch_all.hpp>

#include <format>

using namespace hex;

TEST_CASE("coordinate_axis")
{
    STATIC_CHECK(std::formattable<coordinate_axis, char>);
    STATIC_CHECK(std::formattable<coordinate_axis, wchar_t>);

    CHECK(std::format("{}", coordinate_axis::q) == "q");
    CHECK(std::format("{}", coordinate_axis::r) == "r");
    CHECK(std::format("{}", coordinate_axis::s) == "s");
    CHECK(std::format("{}", coordinate_axis(42)) == "<invalid>");

    CHECK(std::format(L"{}", coordinate_axis::q) == L"q");
    CHECK(std::format(L"{}", coordinate_axis::r) == L"r");
    CHECK(std::format(L"{}", coordinate_axis::s) == L"s");
    CHECK(std::format(L"{}", coordinate_axis(42)) == L"<invalid>");

    CHECK(std::format("{:U}", coordinate_axis::q) == "Q");
    CHECK(std::format("{:U}", coordinate_axis::r) == "R");
    CHECK(std::format("{:U}", coordinate_axis::s) == "S");
    CHECK(std::format("{:U}", coordinate_axis(42)) == "<INVALID>");

    CHECK(std::format(L"{:U}", coordinate_axis::q) == L"Q");
    CHECK(std::format(L"{:U}", coordinate_axis::r) == L"R");
    CHECK(std::format(L"{:U}", coordinate_axis::s) == L"S");
    CHECK(std::format(L"{:U}", coordinate_axis(42)) == L"<INVALID>");
}