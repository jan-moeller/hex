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

#ifndef HEX_CONVEX_POLYGON_VIEW_HPP
#define HEX_CONVEX_POLYGON_VIEW_HPP

#include "hex/convex_polygon_parameters.hpp"
#include "hex/coordinate.hpp"
#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_convex_polygon_iterator.hpp"
#include "hex/detail/detail_hexagon_size.hpp"
#include "hex/vector.hpp"

#include <concepts>
#include <ranges>

#include <cstddef>

namespace hex
{
// A view that models std::ranges::sized_range, std::ranges::common_range, std::ranges::bidirectional_range,
// std::ranges::borrowed_range and std::constant_range, producing all hex positions in a convex polygon.
// Note that this is a bit of an odd view: Even though it provides random access via operator[], it is just a
// bidirectional range. THis is because random access is quite expensive, and additionally comes with some restrictions.
template<detail::arithmetic T>
class convex_polygon_view : public std::ranges::view_interface<convex_polygon_view<T>>
{
  public:
    // Constructs the view with the given parameters.
    constexpr convex_polygon_view(convex_polygon_parameters<T> const& params);

    [[nodiscard]] constexpr auto begin() const noexcept -> detail::convex_polygon_iterator<T>;
    [[nodiscard]] constexpr auto end() const noexcept -> detail::convex_polygon_iterator<T>;

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t;

    // Returns true if the given element is in the range, otherwise false. O(1).
    [[nodiscard]] constexpr auto contains(vector<T> const& v) const noexcept -> bool;
    // Returns an iterator to the given element if it is in the range, otherwise end().
    [[nodiscard]] constexpr auto find(vector<T> const& v) const noexcept -> detail::convex_polygon_iterator<T>;

    // Returns the element at the specified index. Note that this operation, while O(1), has a fairly large constant
    // factor. Results are correct as long as max(q_max-q_min, r_max-r_min, s_max-s_min) <= 2^31 − 1. UB if the given
    // index is out of range.
    [[nodiscard]] constexpr auto operator[](std::size_t idx) const noexcept -> vector<T>;
    // Returns the index of the given element. Note that this operation, while O(1), has a fairly large constant
    // factor. Results are correct as long as max(q_max-q_min, r_max-r_min, s_max-s_min) <= 2^31 − 1. UB if the given
    // element is out of range.
    [[nodiscard]] constexpr auto operator[](vector<T> const& v) const noexcept -> std::size_t;

    // Retrieves the parameters used for construction.
    [[nodiscard]] constexpr auto parameters() const noexcept -> convex_polygon_parameters<T> const&;

    [[nodiscard]] constexpr auto operator==(convex_polygon_view const& other) const -> bool;

  private:
    convex_polygon_parameters<T> m_params;
};

namespace views
{
// A range factory returning a bidirectional view of all positions in a convex polygon.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto convex_polygon(convex_polygon_parameters<T> const& params) -> convex_polygon_view<T>;
} // namespace views

// ------------------------------ implementation below ------------------------------

template<detail::arithmetic T>
constexpr convex_polygon_view<T>::convex_polygon_view(convex_polygon_parameters<T> const& params)
    : m_params(params)
{
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::begin() const noexcept -> detail::convex_polygon_iterator<T>
{
    return detail::convex_polygon_iterator<T>(m_params, vector(m_params.qmin(), m_params.smax()));
}
template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::end() const noexcept -> detail::convex_polygon_iterator<T>
{
    using namespace literals;
    return ++detail::convex_polygon_iterator<T>(m_params, vector(m_params.qmax(), m_params.smin()));
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::size() const noexcept -> std::size_t
{
    return m_params.count();
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::contains(vector<T> const& v) const noexcept -> bool
{
    return m_params.contains(v);
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::find(vector<T> const& v) const noexcept -> detail::convex_polygon_iterator<T>
{
    if (!contains(v))
        return end();
    return detail::convex_polygon_iterator<T>(m_params, v);
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::operator[](std::size_t idx) const noexcept -> vector<T>
{
    auto const [q, r] = detail::index_to_qr(idx,
                                            m_params.qmin().value(),
                                            m_params.rmin().value(),
                                            m_params.smin().value(),
                                            m_params.rmax().value(),
                                            m_params.smax().value());
    return vector(q_coordinate<T>{q}, r_coordinate<T>{r});
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::operator[](vector<T> const& v) const noexcept -> std::size_t
{
    return detail::qr_to_index(v.q().value(),
                               v.r().value(),
                               m_params.qmin().value(),
                               m_params.rmin().value(),
                               m_params.smin().value(),
                               m_params.rmax().value(),
                               m_params.smax().value());
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::parameters() const noexcept -> convex_polygon_parameters<T> const&
{
    return m_params;
}

template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::operator==(convex_polygon_view const& other) const -> bool
{
    return m_params == other.m_params;
}

template<std::signed_integral T>
constexpr auto views::convex_polygon(convex_polygon_parameters<T> const& params) -> convex_polygon_view<T>
{
    return convex_polygon_view(params);
}

} // namespace hex

template<hex::detail::arithmetic T>
inline constexpr bool std::ranges::enable_borrowed_range<hex::convex_polygon_view<T>> = true;

#endif // HEX_CONVEX_POLYGON_VIEW_HPP
