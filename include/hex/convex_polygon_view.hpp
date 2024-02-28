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
template<detail::arithmetic T>
class convex_polygon_view
{
  public:
    constexpr convex_polygon_view(convex_polygon_parameters<T> const& params)
        : m_params(params)
    {
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> detail::bounded_polygon_iterator<T>
    {
        return detail::bounded_polygon_iterator<T>(m_params.rmin(),
                                                   m_params.smin(),
                                                   m_params.rmax(),
                                                   m_params.smax(),
                                                   vector(m_params.qmin(), m_params.smax()));
    }
    [[nodiscard]] constexpr auto end() const noexcept -> detail::bounded_polygon_iterator<T>
    {
        using namespace literals;
        return detail::bounded_polygon_iterator<T>(m_params.rmin(),
                                                   m_params.smin(),
                                                   m_params.rmax(),
                                                   m_params.smax(),
                                                   vector(m_params.qmax() + 1_q, m_params.smin()));
    }
    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t { return m_params.count(); }

  private:
    convex_polygon_parameters<T> m_params;
};

namespace views
{
// A range factory returning a bidirectional view of all positions in a convex polygon.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto convex_polygon(convex_polygon_parameters<T> const& params) -> convex_polygon_view<T>
{
    return convex_polygon_view(params);
}
} // namespace views

} // namespace hex

#endif // HEX_CONVEX_POLYGON_VIEW_HPP
