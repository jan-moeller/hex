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

#ifndef HEX_DETAIL_OFFSET_CONVERSION_HPP
#define HEX_DETAIL_OFFSET_CONVERSION_HPP

#include "hex/coordinate.hpp"
#include "hex/coordinate_axis.hpp"
#include "hex/offset_parity.hpp"
#include "hex/vector.hpp"

#include <array>
#include <utility>

#include <cstddef>

namespace hex::detail
{
template<typename T>
constexpr auto from_odd_q(T x, T y) noexcept -> vector<T>
{
    auto const q = q_coordinate<T>{y};
    auto const r = r_coordinate<T>{x - (y - (y & 1)) / 2};
    return vector<T>(q, r);
}
template<typename T>
constexpr auto from_even_q(T x, T y) noexcept -> vector<T>
{
    auto const q = q_coordinate<T>{y};
    auto const r = r_coordinate<T>{x - (y + (y & 1)) / 2};
    return vector<T>(q, r);
}
template<typename T>
constexpr auto from_odd_r(T x, T y) noexcept -> vector<T>
{
    auto const r = r_coordinate<T>{y};
    auto const s = s_coordinate<T>{x + ((y & 1) - y) / 2};
    return vector<T>(r, s);
}
template<typename T>
constexpr auto from_even_r(T x, T y) noexcept -> vector<T>
{
    auto const r = r_coordinate<T>{y};
    auto const s = s_coordinate<T>{x + (-(y & 1) - y) / 2};
    return vector<T>(r, s);
}
template<typename T>
constexpr auto from_odd_s(T x, T y) noexcept -> vector<T>
{
    auto const q = q_coordinate<T>{x - (y - (y & 1)) / 2};
    auto const s = s_coordinate<T>{y};
    return vector<T>(q, s);
}
template<typename T>
constexpr auto from_even_s(T x, T y) noexcept -> vector<T>
{
    auto const q = q_coordinate<T>{x - (y + (y & 1)) / 2};
    auto const s = s_coordinate<T>{y};
    return vector<T>(q, s);
}
template<typename T>
constexpr auto select_offset_to_cubic_function(coordinate_axis axis, offset_parity parity) -> vector<T> (*)(T, T)
{
    static constexpr std::array<std::array<vector<T> (*)(T, T), 2>, 3> funs{{
        {&from_even_q, &from_odd_q},
        {&from_even_r, &from_odd_r},
        {&from_even_s, &from_odd_s},
    }};
    return funs[std::to_underlying(axis)][std::to_underlying(parity)];
}

template<typename T>
struct apply_offset_conversion
{
    vector<T>   base;           // NOLINT(misc-non-private-member-variables-in-classes)
    std::size_t width;          // NOLINT(misc-non-private-member-variables-in-classes)
    vector<T> (*convert)(T, T); // NOLINT(misc-non-private-member-variables-in-classes)

    constexpr auto operator()(std::size_t idx) const -> vector<T>
    {
        T const x = idx % width;
        T const y = idx / width;
        return convert(x, y) + base;
    }
};
} // namespace hex::detail

#endif // HEX_DETAIL_OFFSET_CONVERSION_HPP
