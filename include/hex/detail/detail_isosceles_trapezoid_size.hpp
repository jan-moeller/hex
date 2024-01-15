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

#ifndef HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP
#define HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP

#include "detail_sqrt.hpp"

#include <algorithm>

#include <cassert>
#include <cstddef>

namespace hex::detail
{
template<typename T>
constexpr auto isosceles_trapezoid_size_from_base_and_height(T base, T height) -> std::size_t
{
    assert(height <= base);
    assert(base >= 0);

    std::size_t b = base;
    std::size_t h = height;

    // h * (b - h/2) + h/2
    auto const h_half = h / 2;
    return h * (b - h_half) + h_half * (h % 2 == 0);
}

template<typename T>
constexpr auto isosceles_trapezoid_size_from_top_and_height(T top, T height) -> std::size_t
{
    return isosceles_trapezoid_size_from_base_and_height(std::max(T{0}, top + height - 1), height);
}
} // namespace hex::detail

#endif // HEX_DETAIL_ISOSCELES_TRAPEZOID_SIZE_HPP
