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
#include "hex/detail/detail_generating_random_access_iterator.hpp"
#include "hex/detail/detail_offset_conversion.hpp"
#include "hex/offset_parity.hpp"
#include "hex/offset_rows_parameters.hpp"
#include "hex/vector.hpp"

#include <array>
#include <concepts>
#include <ranges>

#include <cstddef>

namespace hex
{
// A view that models std::ranges::sized_range, std::ranges::common_range, std::ranges::random_access_range,
// std::ranges::borrowed_range and std::constant_range, producing all hex positions in a semi-rectangular pattern.
template<std::signed_integral T>
class offset_rows_view : public std::ranges::view_interface<offset_rows_view<T>>
{
  public:
    using iterator = detail::generating_random_access_iterator<detail::apply_offset_conversion<T>>;

    // Constructs the view with the given parameters.
    constexpr explicit offset_rows_view(offset_rows_parameters<T> const& parameters);

    [[nodiscard]] constexpr auto begin() const noexcept -> iterator;
    [[nodiscard]] constexpr auto end() const noexcept -> iterator;

    // Returns true if the given element is in the range, otherwise false. O(1).
    [[nodiscard]] constexpr auto contains(vector<T> const& v) const noexcept -> bool;

    // Returns the past-the-end iterator if v is not in the view, otherwise the iterator to v.
    [[nodiscard]] constexpr auto find(vector<T> const& v) const noexcept -> iterator;

    // Reverse random access operator: Returns the index of v. UB if v is not in the view.
    [[nodiscard]] constexpr auto operator[](vector<T> const& v) const noexcept -> std::size_t;

    // Provides access to the parameters passed on construction.
    [[nodiscard]] constexpr auto parameters() const -> offset_rows_parameters<T> const&;

    [[nodiscard]] constexpr auto operator==(offset_rows_view const& other) const -> bool;

  private:
    offset_rows_parameters<T> m_params;
    vector<T> (*m_from_offset)(T, T);
    std::array<T, 2> (*m_to_offset)(vector<T>);

};

namespace views
{
// A range factory returning a random-access view containing all coordinates within a semi-rectangular region.
// The first produced element is always equal to the specified corner.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto offset_rows(offset_rows_parameters<T> const& parameters) -> offset_rows_view<T>;
} // namespace views
} // namespace hex

// ------------------------------ implementation below ------------------------------

template<std::signed_integral T>
constexpr hex::offset_rows_view<T>::offset_rows_view(offset_rows_parameters<T> const& parameters)
    : m_params(parameters)
    , m_from_offset(detail::select_offset_to_cubic_function<T>(m_params.axis(), m_params.parity()))
    , m_to_offset(detail::select_cubic_to_offset_function<T>(m_params.axis(), m_params.parity()))
{
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::begin() const noexcept -> hex::offset_rows_view<T>::iterator
{
    return hex::offset_rows_view<T>::iterator(hex::detail::apply_offset_conversion<T>(m_params.corner(),
                                                                                      m_params.height(),
                                                                                      m_from_offset),
                                              0UZ);
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::end() const noexcept
    -> hex::detail::generating_random_access_iterator<hex::detail::apply_offset_conversion<T>>
{
    return hex::offset_rows_view<T>::iterator(hex::detail::apply_offset_conversion<T>(m_params.corner(),
                                                                                      m_params.height(),
                                                                                      m_from_offset),
                                              m_params.width() * m_params.height());
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::contains(hex::vector<T> const& v) const noexcept -> bool
{
    auto [x, y] = m_to_offset(v - m_params.corner());
    return std::cmp_greater_equal(x, 0) && std::cmp_greater_equal(y, 0) && std::cmp_less(x, m_params.height())
           && std::cmp_less(y, m_params.width());
}
template<std::signed_integral T>
constexpr auto
hex::offset_rows_view<T>::find(hex::vector<T> const& v) const noexcept -> hex::offset_rows_view<T>::iterator
{
    if (!contains(v))
        return end();
    return begin() + (*this)[v];
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::operator[](hex::vector<T> const& v) const noexcept -> std::size_t
{
    auto const [x, y] = this->m_to_offset(v - m_params.corner());
    return x + this->m_params.height() * y;
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::parameters() const -> hex::offset_rows_parameters<T> const&
{
    return m_params;
}
template<std::signed_integral T>
constexpr auto hex::offset_rows_view<T>::operator==(hex::offset_rows_view<T> const& other) const -> bool
{
    return m_params == other.m_params && m_from_offset == other.m_from_offset && m_to_offset == other.m_to_offset;
}

template<std::signed_integral T>
constexpr auto hex::views::offset_rows(hex::offset_rows_parameters<T> const& parameters) -> hex::offset_rows_view<T>
{
    return offset_rows_view<T>(parameters);
}

template<std::signed_integral T>
inline constexpr bool std::ranges::enable_borrowed_range<hex::offset_rows_view<T>> = true;

#endif // HEX_OFFSET_ROWS_VIEW_HPP
