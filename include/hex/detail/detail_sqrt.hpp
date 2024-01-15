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

#ifndef HEX_DETAIL_SQRT_HPP
#define HEX_DETAIL_SQRT_HPP

#include <algorithm>
#include <bit>
#include <concepts>

namespace hex::detail
{
template<std::unsigned_integral T>
constexpr auto lower_bound_guess_sqrt(T const n) -> T
{
    if (n == 0)
        return 0;
    // sqrt(x) = 2^(x/2) = 2^(x>>1) = 1<<(x>>1) for any power of two x.
    return 1U << ((std::bit_width(n) - 1U) >> 1U);
}

template<std::unsigned_integral T>
constexpr auto approx_sqrt(T const n) -> T
{
    T a = lower_bound_guess_sqrt(n);
    T b = n;

    while (std::max(a, b) > std::min(a, b) + 1U)
    {
        b = n / a;
        a = (a + b) / 2U;
    }

    // Depending on n, a is now exactly sqrt(n), or ⌈sqrt(n)⌉, or ⌊sqrt(n)⌋.
    return a;
}

template<std::unsigned_integral T>
constexpr auto floor_sqrt(T const n) -> T
{
    auto const a = approx_sqrt(n);
    // If approx_sqrt gave ⌈sqrt(n)⌉, decrement by 1 to get ⌊sqrt(n)⌋.
    return a - (a * a > n);
}

template<std::unsigned_integral T>
constexpr auto ceil_sqrt(T const n) -> T
{
    auto const a = approx_sqrt(n);
    // If approx_sqrt gave ⌊sqrt(n)⌋, increment by 1 to get ⌈sqrt(n)⌉.
    return a + (a * a < n);
}
} // namespace hex::detail

#endif // HEX_DETAIL_SQRT_HPP
