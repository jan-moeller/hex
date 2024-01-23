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

#ifndef HEX_DETAIL_HEXAGON_BOUNDS_HPP
#define HEX_DETAIL_HEXAGON_BOUNDS_HPP

#include <algorithm>
#include <concepts>

namespace hex::detail
{
// Makes sure the passed bounds are tight. Returns false, if the bounds were invalid, otherwise true.
template<std::integral T>
constexpr auto tighten_bounds(T& q_min, T& r_min, T& s_min, T& q_max, T& r_max, T& s_max) -> bool
{
    using std::swap;
    if (q_min > q_max)
        swap(q_min, q_max);
    if (r_min > r_max)
        swap(r_min, r_max);
    if (s_min > s_max)
        swap(s_min, s_max);

    q_min = std::max(q_min, -r_max - s_max);
    r_min = std::max(r_min, -q_max - s_max);
    s_min = std::max(s_min, -q_max - r_max);
    q_max = std::min(q_max, -r_min - s_min);
    r_max = std::min(r_max, -q_min - s_min);
    s_max = std::min(s_max, -q_min - r_min);

    return !(q_min > q_max || r_min > r_max || s_min > s_max);
}
} // namespace hex::detail

#endif // HEX_DETAIL_HEXAGON_BOUNDS_HPP
