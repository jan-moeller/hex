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

#ifndef HEX_LINE_VIEW_HPP
#define HEX_LINE_VIEW_HPP

#include "hex/vector/vector.hpp"
#include "hex/views/line/detail/detail_line_iterator.hpp"

#include <concepts>
#include <ranges>

#include <cstddef>

namespace hex
{
// A view that models std::ranges::sized_range, std::ranges::common_range, std::ranges::forward_range,
// std::ranges::borrowed_range and std::constant_range, producing all hex positions on a line between to positions.
template<std::signed_integral T>
class line_view : public std::ranges::view_interface<line_view<T>>
{
  public:
    // Constructs the view with the given parameters.
    constexpr line_view(vector<T> const& from, vector<T> const& to);

    [[nodiscard]] constexpr auto begin() const noexcept -> detail::line_iterator<T>;
    [[nodiscard]] constexpr auto end() const noexcept -> detail::line_iterator<T>;

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t;

    [[nodiscard]] constexpr auto operator==(line_view const& other) const -> bool = default;

  private:
    vector<T> m_from;
    vector<T> m_to;
};

namespace views
{
// A range factory returning a view containing the hex positions on a line between two positions.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto line(vector<T> const& from, vector<T> const& to) -> line_view<T>;
} // namespace views
} // namespace hex

// ------------------------------ implementation below ------------------------------

template<std::signed_integral T> // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr hex::line_view<T>::line_view(hex::vector<T> const& from, hex::vector<T> const& to)
    : m_from(from)
    , m_to(to)
{
}
template<std::signed_integral T>
constexpr auto hex::line_view<T>::begin() const noexcept -> hex::detail::line_iterator<T>
{
    return hex::detail::line_iterator<T>(m_from, m_to);
}
template<std::signed_integral T>
constexpr auto hex::line_view<T>::end() const noexcept -> hex::detail::line_iterator<T>
{
    return hex::detail::line_iterator<T>();
}
template<std::signed_integral T>
constexpr auto hex::line_view<T>::size() const noexcept -> std::size_t
{
    return distance(m_from, m_to);
}

template<std::signed_integral T>
constexpr auto hex::views::line(hex::vector<T> const& from, hex::vector<T> const& to) -> hex::line_view<T>
{
    return hex::line_view<T>(from, to);
}

template<std::signed_integral T>
inline constexpr bool std::ranges::enable_borrowed_range<hex::line_view<T>> = true;

#endif // HEX_LINE_VIEW_HPP
