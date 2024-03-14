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

#ifndef HEX_OFFSET_ROWS_VIEW_HPP
#define HEX_OFFSET_ROWS_VIEW_HPP

#include "hex/coordinate_axis.hpp"
#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_offset_conversion.hpp"
#include "hex/offset_parity.hpp"
#include "hex/vector.hpp"

#include <concepts>
#include <ranges>

#include <cstddef>

namespace hex
{
// The type returned by views::offset_rows.
template<detail::arithmetic T>
using offset_rows_view = std::ranges::transform_view<std::ranges::iota_view<std::size_t, std::size_t>,
                                                     detail::apply_offset_conversion<T>>;

namespace views
{
// A range factory returning a random-access view containing all coordinates within a "rectangular" region.
//   axis determines the axis which determines whether a row is offset or not.
//   parity determines whether even or odd rows are offset along axis.
//   width and height determine the size of the region - it starts at base, and extends by width along axis in positive
//   direction, and by height into the orthogonal direction (which is between axis and the next axis in q, r, s order).
// The first contained element is always equal to base.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto offset_rows(coordinate_axis axis,
                                         offset_parity   parity,
                                         std::size_t     width,
                                         std::size_t     height,
                                         vector<T>       base = {}) -> offset_rows_view<T>
{
    return std::views::iota(0ZU, width * height)
           | std::views::transform(
               detail::apply_offset_conversion<T>{base,
                                                  height,
                                                  detail::select_offset_to_cubic_function<T>(axis, parity)});
}
} // namespace views
} // namespace hex

#endif // HEX_OFFSET_ROWS_VIEW_HPP
