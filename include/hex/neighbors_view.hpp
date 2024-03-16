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

#ifndef HEX_NEIGHBORS_VIEW_HPP
#define HEX_NEIGHBORS_VIEW_HPP

#include "hex/detail/detail_neighbors.hpp"
#include "hex/vector.hpp"

#include <array>
#include <concepts>
#include <ranges>

#include <cstdint>

namespace hex
{
template<std::signed_integral T> // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
using neighbors_view = std::ranges::transform_view<std::ranges::ref_view<std::array<vector<std::int8_t>, 6> const>,
                                                   detail::apply_neighbor<T>>;

namespace views
{
// A range factory returning a view containing the 6 neighbors of the given vector. This view meets the requirements of
// random_access_range, sized_range, constant_range, and common_range.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto neighbors(vector<T> const& center = {}) -> neighbors_view<T>
{
    return detail::neighbors | std::views::transform(detail::apply_neighbor<T>{center});
}

// A range factory returning a view containing the 6 diagonal neighbors of the given vector. This view meets the
// requirements of random_access_range, sized_range, constant_range, and common_range.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto diagonals(vector<T> const& center = {}) -> neighbors_view<T>
{
    return detail::diagonals | std::views::transform(detail::apply_neighbor<T>{center});
}
} // namespace views
} // namespace hex

#endif // HEX_NEIGHBORS_VIEW_HPP
