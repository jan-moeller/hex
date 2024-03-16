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

#ifndef HEX_CONVEX_POLYGON_PARAMETERS_HPP
#define HEX_CONVEX_POLYGON_PARAMETERS_HPP

#include "hex/vector/coordinate.hpp"
#include "hex/vector/vector.hpp"
#include "hex/views/convex_polygon/detail/detail_hexagon_size.hpp"

#include <concepts>
#include <stdexcept>

#include <cstddef>

namespace hex
{
// Holds the 6 half plane bounds describing a convex hex grid polygon. Every instance is valid, and all bounds are
// tight.
template<std::signed_integral T = int>
class convex_polygon_parameters
{
  public:
    // Describes a convex polygon by supplying 6 half-planes, 2 pairs of which are parallel. All bounds are inclusive.
    // If the bounds aren't tight, or do not designate a convex polygon (e.g. because the common subset is empty),
    // std::invalid_argument is thrown.
    constexpr convex_polygon_parameters(q_coordinate<T> q_min,
                                        r_coordinate<T> r_min,
                                        s_coordinate<T> s_min,
                                        q_coordinate<T> q_max,
                                        r_coordinate<T> r_max,
                                        s_coordinate<T> s_max);

    [[nodiscard]] constexpr auto qmin() const noexcept -> q_coordinate<T>; // Returns the lower q bound (inclusive).
    [[nodiscard]] constexpr auto qmax() const noexcept -> q_coordinate<T>; // Returns the lower r bound (inclusive).
    [[nodiscard]] constexpr auto rmin() const noexcept -> r_coordinate<T>; // Returns the lower s bound (inclusive).
    [[nodiscard]] constexpr auto rmax() const noexcept -> r_coordinate<T>; // Returns the upper q bound (inclusive).
    [[nodiscard]] constexpr auto smin() const noexcept -> s_coordinate<T>; // Returns the upper r bound (inclusive).
    [[nodiscard]] constexpr auto smax() const noexcept -> s_coordinate<T>; // Returns the upper s bound (inclusive).

    // Computes the number of tiles inside this polygon.
    [[nodiscard]] constexpr auto count() const noexcept -> std::size_t;

    // Returns true if the passed position is within the bounds of this polygon, false otherwise.
    [[nodiscard]] constexpr auto contains(vector<T> const& v) const noexcept -> bool;

    [[nodiscard]] constexpr auto operator==(convex_polygon_parameters const& other) const noexcept -> bool = default;

  private:
    T m_qmin;
    T m_rmin;
    T m_smin;
    T m_qmax;
    T m_rmax;
    T m_smax;
};

// Describes a regular hexagon by supplying its radius and center. Throws std::invalid_argument if the radius is
// negative. A radius of 0 leads to only the center being within bounds.
template<std::signed_integral T = int>
constexpr auto make_regular_hexagon_parameters(T radius, vector<T> const& center = {}) -> convex_polygon_parameters<T>;

// Describes a regular triangle by supplying 3 limiting half planes.
template<std::signed_integral T = int>
constexpr auto make_regular_triangle_parameters(q_coordinate<T> q, r_coordinate<T> r, s_coordinate<T> s)
    -> convex_polygon_parameters<T>;

// ------------------------------ implementation below ------------------------------

template<std::signed_integral T>
constexpr convex_polygon_parameters<T>::convex_polygon_parameters(q_coordinate<T> q_min,
                                                                  r_coordinate<T> r_min,
                                                                  s_coordinate<T> s_min,
                                                                  q_coordinate<T> q_max,
                                                                  r_coordinate<T> r_max,
                                                                  s_coordinate<T> s_max)

    : m_qmin(q_min.value())
    , m_rmin(r_min.value())
    , m_smin(s_min.value())
    , m_qmax(q_max.value())
    , m_rmax(r_max.value())
    , m_smax(s_max.value())
{
    if (m_qmin < -m_smax - m_rmax || m_smax > -m_qmin - m_rmin || m_rmin < -m_smax - m_qmax || m_qmax > -m_smin - m_rmin
        || m_smin < -m_rmax - m_qmax || m_rmax > -m_smin - m_qmin)
        throw std::invalid_argument("Invalid bounds for convex polygon");
}

template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::qmin() const noexcept -> q_coordinate<T>
{
    return q_coordinate<T>{m_qmin};
}
template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::qmax() const noexcept -> q_coordinate<T>
{
    return q_coordinate<T>{m_qmax};
}
template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::rmin() const noexcept -> r_coordinate<T>
{
    return r_coordinate<T>{m_rmin};
}
template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::rmax() const noexcept -> r_coordinate<T>
{
    return r_coordinate<T>{m_rmax};
}
template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::smin() const noexcept -> s_coordinate<T>
{
    return s_coordinate<T>{m_smin};
}
template<std::signed_integral T>
[[nodiscard]] constexpr auto convex_polygon_parameters<T>::smax() const noexcept -> s_coordinate<T>
{
    return s_coordinate<T>{m_smax};
}

template<std::signed_integral T>
constexpr auto convex_polygon_parameters<T>::count() const noexcept -> std::size_t
{
    return detail::hexagon_size(m_qmin, m_rmin, m_smin, m_qmax, m_rmax, m_smax);
};

template<std::signed_integral T>
constexpr auto convex_polygon_parameters<T>::contains(vector<T> const& v) const noexcept -> bool
{
    auto const q = v.q().value();
    auto const r = v.r().value();
    auto const s = v.s().value();
    return q >= m_qmin && q <= m_qmax && r >= m_rmin && r <= m_rmax && s >= m_smin && s <= m_smax;
}

template<std::signed_integral T>
constexpr auto make_regular_hexagon_parameters(T radius, vector<T> const& center) -> convex_polygon_parameters<T>
{
    return convex_polygon_parameters(center.q() - q_coordinate<T>{radius},
                                     center.r() - r_coordinate<T>{radius},
                                     center.s() - s_coordinate<T>{radius},
                                     center.q() + q_coordinate<T>{radius},
                                     center.r() + r_coordinate<T>{radius},
                                     center.s() + s_coordinate<T>{radius});
};

template<std::signed_integral T>
constexpr auto make_regular_triangle_parameters(q_coordinate<T> q, r_coordinate<T> r, s_coordinate<T> s)
    -> convex_polygon_parameters<T>
{
    auto q_max = q_coordinate<T>(-r.value() - s.value());
    auto r_max = r_coordinate<T>(-q.value() - s.value());
    auto s_max = s_coordinate<T>(-q.value() - r.value());
    if (q_max < q)
        std::swap(q_max, q);
    if (r_max < r)
        std::swap(r_max, r);
    if (s_max < s)
        std::swap(s_max, s);
    return convex_polygon_parameters(q, r, s, q_max, r_max, s_max);
};

} // namespace hex

#endif // HEX_CONVEX_POLYGON_PARAMETERS_HPP
