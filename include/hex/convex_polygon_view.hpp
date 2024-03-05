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
#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_bounded_polygon_iterator.hpp"
#include "hex/vector.hpp"

#include <concepts>

#include <cstddef>

namespace hex
{
// A view that models std::ranges::sized_range, std::ranges::common_range, std::ranges::bidirectional_range,
// std::ranges::borrowed_range and std::constant_range, producing all hex positions in a convex polygon.
template<detail::arithmetic T>
class convex_polygon_view
{
  public:
    // Constructs the view with the given parameters.
    constexpr convex_polygon_view(convex_polygon_parameters<T> const& params);

    [[nodiscard]] constexpr auto begin() const noexcept -> detail::bounded_polygon_iterator<T>;
    [[nodiscard]] constexpr auto end() const noexcept -> detail::bounded_polygon_iterator<T>;

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t;

    // Returns true if the given element is in the range, otherwise false. O(1).
    [[nodiscard]] constexpr auto contains(vector<T> const& v) const noexcept -> bool;

    // Retrieves the parameters used for construction.
    [[nodiscard]] constexpr auto parameters() const noexcept -> convex_polygon_parameters<T> const&;

    [[nodiscard]] constexpr auto operator==(convex_polygon_view const&) const -> bool = default;

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
constexpr auto convex_polygon_view<T>::begin() const noexcept -> detail::bounded_polygon_iterator<T>
{
    return detail::bounded_polygon_iterator<T>(m_params.rmin(),
                                               m_params.smin(),
                                               m_params.rmax(),
                                               m_params.smax(),
                                               vector(m_params.qmin(), m_params.smax()));
}
template<detail::arithmetic T>
constexpr auto convex_polygon_view<T>::end() const noexcept -> detail::bounded_polygon_iterator<T>
{
    using namespace literals;
    return ++detail::bounded_polygon_iterator<T>(m_params.rmin(),
                                                 m_params.smin(),
                                                 m_params.rmax(),
                                                 m_params.smax(),
                                                 vector(m_params.qmax(), m_params.smin()));
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
constexpr auto convex_polygon_view<T>::parameters() const noexcept -> convex_polygon_parameters<T> const&
{
    return m_params;
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
