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

#ifndef HEX_VECTOR_HPP
#define HEX_VECTOR_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_narrowing.hpp"
#include "hex/vector/coordinate.hpp"
#include "hex/vector/coordinate_axis.hpp"
#include "hex/vector/detail/detail_vector_iterator.hpp"
#include "hex/vector/rotation_steps.hpp"

#include <array>
#include <concepts>
#include <iterator>
#include <numbers>
#include <type_traits>
#include <utility>

#include <cmath>
#include <cstdint>

namespace hex
{
// A hexagonal grid vector holding 3 coordinates q, r and s, where q + r + s = 0.
// Identifiers and documentation assume that the axes are designated in clockwise order, like this:
//    ________    //
//   / +s  -r \   //
//  /          \  //
// ⟨ -q   0  +q ⟩ //
//  \          /  //
//   \ +r  -s /   //
//    ‾‾‾‾‾‾‾‾    //
//
// Vectors are random-access ranges of (axis, value) pairs. They also provide a map-like interface to access value by
// axis.
template<detail::arithmetic T = int>
class vector
{
  public:
    using key_type               = coordinate_axis;
    using mapped_type            = T;
    using value_type             = std::pair<key_type const, mapped_type>;
    using size_type              = std::uint8_t;
    using difference_type        = std::int8_t;
    using iterator               = detail::vector_iterator<vector<T>>;
    using const_iterator         = iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    template<coordinate_axis Axis>
    using coordinate   = coordinate<Axis, T>;
    using q_coordinate = coordinate<coordinate_axis::q>;
    using r_coordinate = coordinate<coordinate_axis::r>;
    using s_coordinate = coordinate<coordinate_axis::s>;

    // Construct the zero-vector.
    constexpr vector() = default;

    // Construct a vector from q and r coordinates.
    constexpr vector(q_coordinate q, r_coordinate r);

    // Construct a vector from q and s coordinates.
    constexpr vector(q_coordinate q, s_coordinate s);

    // Construct a vector from r and s coordinates.
    constexpr vector(r_coordinate r, s_coordinate s);

    // Construct a vector from all 3 coordinates. Throws std::invalid_argument if q + r + s != 0.
    constexpr vector(q_coordinate q, r_coordinate r, s_coordinate s);

    // Constructs a vector from another vector of a different underlying type.
    // This operation is implicit if it is non-narrowing. Otherwise, an explicit cast is required.
    template<detail::arithmetic U>
        requires(!std::same_as<T, U>)
    constexpr explicit(detail::narrowing<U, T>) vector(vector<U> vec); // NOLINT(google-explicit-constructor)

    [[nodiscard]] constexpr auto operator==(vector const& rhs) const noexcept -> bool = default;
    [[nodiscard]] constexpr auto operator<=>(vector const& rhs) const noexcept        = default;

    // Sets q and r coordinate, automatically updating s.
    constexpr void set(q_coordinate q, r_coordinate r);
    // Sets q and s coordinate, automatically updating r.
    constexpr void set(q_coordinate q, s_coordinate s);
    // Sets r and s coordinate, automatically updating q.
    constexpr void set(r_coordinate r, s_coordinate s);
    // Sets all 3 coordinates. Throws std::invalid_argument if q + r + s != 0.
    constexpr void set(q_coordinate q, r_coordinate r, s_coordinate s);

    [[nodiscard]] constexpr auto q() const noexcept -> q_coordinate;
    [[nodiscard]] constexpr auto r() const noexcept -> r_coordinate;
    [[nodiscard]] constexpr auto s() const noexcept -> s_coordinate;

    // Returns the axis and value along that axis at position i, where i is in [0, 2].
    [[nodiscard]] constexpr auto operator[](size_type const& i) const -> value_type;
    // Returns the value along the given axis.
    [[nodiscard]] constexpr auto operator[](key_type const& key) const -> mapped_type;

    [[nodiscard]] constexpr auto begin() noexcept -> iterator;
    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator;
    [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator;

    [[nodiscard]] constexpr auto end() noexcept -> iterator;
    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator;
    [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator;

    [[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator;
    [[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator;
    [[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator;

    [[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator;
    [[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator;
    [[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator;

    // Returns 3.
    [[nodiscard]] constexpr auto size() const noexcept -> size_type;

    [[nodiscard]] constexpr auto operator+() const -> vector<decltype(+T{})>;
    [[nodiscard]] constexpr auto operator-() const -> vector<decltype(-T{})>;

    constexpr auto operator+=(vector const& rhs) -> vector&;
    constexpr auto operator-=(vector const& rhs) -> vector&;
    constexpr auto operator*=(T const& rhs) -> vector&;
    constexpr auto operator/=(T const& rhs) -> vector&;

    // Returns the distance in hex grid tiles from the origin.
    [[nodiscard]] constexpr auto norm() const noexcept -> T;

  private:
    q_coordinate m_q;
    r_coordinate m_r;
};

template<typename Q, typename R, typename S>
vector(q_coordinate<Q>, r_coordinate<R>, s_coordinate<S>) -> vector<std::common_type_t<Q, R, S>>;
template<typename Q, typename R>
vector(q_coordinate<Q>, r_coordinate<R>) -> vector<std::common_type_t<Q, R>>;
template<typename Q, typename S>
vector(q_coordinate<Q>, s_coordinate<S>) -> vector<std::common_type_t<Q, S>>;
template<typename R, typename S>
vector(r_coordinate<R>, s_coordinate<S>) -> vector<std::common_type_t<R, S>>;

// Sums two coordinates.
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator+(vector<Lhs> const& lhs, vector<Rhs> const& rhs)
    -> vector<decltype(Lhs{} + Rhs{})>;

// Subtracts a vector from another.
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator-(vector<Lhs> const& lhs, vector<Rhs> const& rhs)
    -> vector<decltype(Lhs{} - Rhs{})>;

// Multiplies a vector by a factor.
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(vector<Lhs> const& lhs, Rhs const& rhs) -> vector<decltype(Lhs{} * Rhs{})>;
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(Lhs const& lhs, vector<Rhs> const& rhs) -> vector<decltype(Lhs{} * Rhs{})>;

// Divides a vector by a factor.
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator/(vector<Lhs> const& lhs, Rhs const& rhs) -> vector<decltype(Lhs{} / Rhs{})>;

// Cast to a vector of different underlying type.
template<detail::arithmetic T, typename U>
[[nodiscard]] constexpr auto coordinate_cast(vector<U> const& vec) -> vector<T>;

// Computes the hex grid distance between two coordinate vectors.
template<detail::arithmetic T = int, detail::arithmetic U = int>
    requires(std::signed_integral<T> == std::signed_integral<U> && std::floating_point<T> == std::floating_point<U>)
[[nodiscard]] constexpr auto distance(vector<T> const& from, vector<U> const& to) -> std::common_type_t<T, U>;

// Returns true if the two hex coordinates designate adjacent tiles, otherwise false.
template<std::signed_integral T = int>
[[nodiscard]] constexpr auto adjacent(vector<T> const& from, vector<T> const& to) -> bool;

// Rounds a fractional vector to the closest integral vector.
template<std::integral T = int, detail::arithmetic U>
[[nodiscard]] constexpr auto round(vector<U> const& vec) -> vector<T>;

// Linear interpolates between two fractional hex grid vectors.
template<std::floating_point T>
[[nodiscard]] constexpr auto lerp(vector<T> const& a, vector<T> const& b, T t) -> vector<T>;

// Rotates a vector in 60° steps either clockwise (rotations > 0) or counter-clockwise (rotations < 0).
template<detail::arithmetic T>
[[nodiscard]] constexpr auto rotate(vector<T> vec, rotation_steps steps) -> vector<T>;

// Reflects the vector across an axis.
template<detail::arithmetic T>
[[nodiscard]] constexpr auto reflect(vector<T> vec, coordinate_axis axis) -> vector<T>;

// Returns the cartesian coordinates of the given hex vector, assuming the cartesian grid's origin coincides with the
// hex grid origin, the x-axis coincides with the q-axis, and the y-axis is between the +r and -s axis. (0, 0, 0) is
// mapped to (0, 0), (1, 0, -1) is mapped to (1.5, sqrt(3)/2), and (0, 1, -1) is mapped to (0, sqrt(3)).
template<typename R = double, detail::arithmetic T>
[[nodiscard]] constexpr auto to_cartesian(vector<T> vec) -> std::array<R, 2>;

// Returns the hex grid vector of the given cartesian coordinates, assuming the cartesian grid's origin coincides with
// the hex grid origin, the x-axis coincides with the q-axis, and the y-axis is between the +r and -s axis. (0, 0) is
// mapped to (0, 0, 0), (1.5, sqrt(3)/2) is mapped to (1, 0, -1), and (0, sqrt(3)) is mapped to (0, 1, -1).
template<typename R = double, detail::arithmetic T>
[[nodiscard]] constexpr auto from_cartesian(std::array<T, 2> pos) -> vector<R>;

// ------------------------------ implementation below ------------------------------

template<typename Lhs, typename Rhs>
constexpr auto operator+(vector<Lhs> const& lhs, vector<Rhs> const& rhs) -> vector<decltype(Lhs{} + Rhs{})>
{
    return vector<decltype(Lhs{} + Rhs{})>{lhs.q() + rhs.q(), lhs.r() + rhs.r()};
}
template<typename Lhs, typename Rhs>
constexpr auto operator-(vector<Lhs> const& lhs, vector<Rhs> const& rhs) -> vector<decltype(Lhs{} - Rhs{})>
{
    return vector<decltype(Lhs{} - Rhs{})>{lhs.q() - rhs.q(), lhs.r() - rhs.r()};
}
template<typename Lhs, typename Rhs>
constexpr auto operator*(vector<Lhs> const& lhs, Rhs const& rhs) -> vector<decltype(Lhs{} * Rhs{})>
{
    return vector<decltype(Lhs{} * Rhs{})>{lhs.q() * rhs, lhs.r() * rhs};
}
template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs const& lhs, vector<Rhs> const& rhs) -> vector<decltype(Lhs{} * Rhs{})>
{
    return vector<decltype(Lhs{} * Rhs{})>{lhs * rhs.q(), lhs * rhs.r()};
}
template<typename Lhs, typename Rhs>
constexpr auto operator/(vector<Lhs> const& lhs, Rhs const& rhs) -> vector<decltype(Lhs{} / Rhs{})>
{
    return vector<decltype(Lhs{} / Rhs{})>{lhs.q() / rhs, lhs.r() / rhs};
}
template<detail::arithmetic T>
constexpr vector<T>::vector(q_coordinate q, r_coordinate r)
    : m_q(q)
    , m_r(r)
{
}
template<detail::arithmetic T>
constexpr vector<T>::vector(q_coordinate q, s_coordinate s)
    : m_q(q)
    , m_r(r_coordinate(-s.value() - q.value()))
{
}
template<detail::arithmetic T>
constexpr vector<T>::vector(r_coordinate r, s_coordinate s)
    : m_q(q_coordinate(-r.value() - s.value()))
    , m_r(r)
{
}
template<detail::arithmetic T>
constexpr vector<T>::vector(q_coordinate q, r_coordinate r, s_coordinate s)
    : m_q(q)
    , m_r(r)
{
    if ((q.value() + r.value() + s.value()) != 0)
        throw std::invalid_argument{"hex grid coordinates sum must be 0"};
}
template<detail::arithmetic T>
template<detail::arithmetic U>
    requires(!std::same_as<T, U>)
constexpr vector<T>::vector(vector<U> vec) // NOLINT(google-explicit-constructor)
    : vector(q_coordinate(vec.q()), r_coordinate(vec.r()))
{
}
template<detail::arithmetic T>
constexpr void vector<T>::set(q_coordinate q, r_coordinate r)
{
    m_q = q;
    m_r = r;
}
template<detail::arithmetic T>
constexpr void vector<T>::set(q_coordinate q, s_coordinate s)
{
    m_q = q;
    m_r = r_coordinate(-s.value() - q.value());
}
template<detail::arithmetic T>
constexpr void vector<T>::set(r_coordinate r, s_coordinate s)
{
    m_q = q_coordinate(-r.value() - s.value());
    m_r = r;
}
template<detail::arithmetic T>
constexpr void vector<T>::set(q_coordinate q, r_coordinate r, s_coordinate s)
{
    if ((q.value() + r.value() + s.value()) != 0)
        throw std::invalid_argument{"hex grid coordinates sum must be 0"};
    m_q = q;
    m_r = r;
}

template<detail::arithmetic T>
constexpr auto vector<T>::q() const noexcept -> q_coordinate
{
    return m_q;
}
template<detail::arithmetic T>
constexpr auto vector<T>::r() const noexcept -> r_coordinate
{
    return m_r;
}
template<detail::arithmetic T>
constexpr auto vector<T>::s() const noexcept -> s_coordinate
{
    return s_coordinate(-m_q.value() - m_r.value());
}

template<detail::arithmetic T>
constexpr auto vector<T>::operator[](size_type const& i) const -> value_type
{
    return begin()[i];
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator[](key_type const& key) const -> mapped_type
{
    return (*this)[std::to_underlying(key)].second;
}

template<detail::arithmetic T>
constexpr auto vector<T>::begin() noexcept -> iterator
{
    return iterator(this, 0);
}
template<detail::arithmetic T>
constexpr auto vector<T>::begin() const noexcept -> const_iterator
{
    return const_iterator(this, 0);
}
template<detail::arithmetic T>
constexpr auto vector<T>::cbegin() const noexcept -> const_iterator
{
    return const_iterator(this, 0);
}
template<detail::arithmetic T>
constexpr auto vector<T>::end() noexcept -> iterator
{
    return iterator(this, 3);
}
template<detail::arithmetic T>
constexpr auto vector<T>::end() const noexcept -> const_iterator
{
    return const_iterator(this, 3);
}
template<detail::arithmetic T>
constexpr auto vector<T>::cend() const noexcept -> const_iterator
{
    return const_iterator(this, 3);
}
template<detail::arithmetic T>
constexpr auto vector<T>::rbegin() noexcept -> reverse_iterator
{
    return reverse_iterator(end());
}
template<detail::arithmetic T>
constexpr auto vector<T>::rbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}
template<detail::arithmetic T>
constexpr auto vector<T>::crbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}
template<detail::arithmetic T>
constexpr auto vector<T>::rend() noexcept -> reverse_iterator
{
    return reverse_iterator(begin());
}
template<detail::arithmetic T>
constexpr auto vector<T>::rend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}
template<detail::arithmetic T>
constexpr auto vector<T>::crend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}
template<detail::arithmetic T>
constexpr auto vector<T>::size() const noexcept -> vector::size_type
{
    return 3;
}

template<detail::arithmetic T>
constexpr auto vector<T>::operator+() const -> vector<decltype(+T{})>
{
    return *this;
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator-() const -> vector<decltype(-T{})>
{
    return {-m_q, -m_r};
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator+=(vector const& rhs) -> vector&
{
    m_q += rhs.m_q;
    m_r += rhs.m_r;
    return *this;
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator-=(vector const& rhs) -> vector&
{
    m_q -= rhs.m_q;
    m_r -= rhs.m_r;
    return *this;
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator*=(T const& rhs) -> vector&
{
    m_q *= rhs;
    m_r *= rhs;
    return *this;
}
template<detail::arithmetic T>
constexpr auto vector<T>::operator/=(T const& rhs) -> vector&
{
    m_q /= rhs;
    m_r /= rhs;
    return *this;
}
template<detail::arithmetic T>
constexpr auto vector<T>::norm() const noexcept -> T
{
    return std::max(std::max(std::abs(q().value()), std::abs(r().value())), std::abs(s().value()));
}

template<detail::arithmetic T, detail::arithmetic U>
constexpr auto coordinate_cast(vector<U> const& vec) -> vector<T>
{
    using q_coord = vector<T>::q_coordinate;
    using r_coord = vector<T>::r_coordinate;
    return vector<T>(q_coord{static_cast<T>(vec.q().value())}, r_coord{static_cast<T>(vec.r().value())});
}

template<detail::arithmetic T, detail::arithmetic U>
    requires(std::signed_integral<T> == std::signed_integral<U> && std::floating_point<T> == std::floating_point<U>)
constexpr auto distance(vector<T> const& from, vector<U> const& to) -> std::common_type_t<T, U>
{
    return (to - from).norm();
}

template<std::signed_integral T>
constexpr auto adjacent(vector<T> const& from, vector<T> const& to) -> bool
{
    return distance(from, to) == 1;
}

template<std::integral T, detail::arithmetic U>
constexpr auto round(vector<U> const& vec) -> vector<T>
{
    if constexpr (std::integral<U>)
        return vector<T>{vec.q(), vec.r()};
    else
    {
        auto const q = std::round(vec.q().value());
        auto const r = std::round(vec.r().value());
        auto const s = std::round(vec.s().value());

        auto const qd = std::abs(q - vec.q().value());
        auto const rd = std::abs(r - vec.r().value());
        auto const sd = std::abs(s - vec.s().value());

        using q_coord = vector<T>::q_coordinate;
        using r_coord = vector<T>::r_coordinate;
        using s_coord = vector<T>::s_coordinate;

        if (qd > rd && qd > sd)
            return {r_coord(static_cast<T>(r)), s_coord(static_cast<T>(s))};
        if (rd > sd)
            return {q_coord(static_cast<T>(q)), s_coord(static_cast<T>(s))};
        return {q_coord(static_cast<T>(q)), r_coord(static_cast<T>(r))};
    }
}

template<std::floating_point T>
constexpr auto lerp(vector<T> const& a, vector<T> const& b, T t) -> vector<T>
{
    using q_coord = vector<T>::q_coordinate;
    using r_coord = vector<T>::r_coordinate;
    return vector<T>{q_coord{std::lerp(a.q().value(), b.q().value(), t)},
                     r_coord{std::lerp(a.r().value(), b.r().value(), t)}};
}

template<detail::arithmetic T>
[[nodiscard]] constexpr auto rotate(vector<T> vec, rotation_steps steps) -> vector<T>
{
    using q_coord = vector<T>::q_coordinate;
    using r_coord = vector<T>::r_coordinate;
    switch (steps.clockwise_steps())
    {
    case 0: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        break;
    case 1: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        vec.set(q_coord(-vec.r()), r_coord(-vec.s()));
        break;
    case 2: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        vec.set(q_coord(vec.s()), r_coord(vec.q()));
        break;
    case 3: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        vec.set(q_coord(-vec.q()), r_coord(-vec.r()));
        break;
    case 4: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        vec.set(q_coord(vec.r()), r_coord(vec.s()));
        break;
    case 5: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        vec.set(q_coord(-vec.s()), r_coord(-vec.q()));
        break;
    default:
        std::unreachable();
    }
    return vec;
}

template<detail::arithmetic T>
[[nodiscard]] constexpr auto reflect(vector<T> vec, coordinate_axis axis) -> vector<T>
{
    using q_coord = vector<T>::q_coordinate;
    using r_coord = vector<T>::r_coordinate;
    switch (axis)
    {
        using enum coordinate_axis;
    case q:
        vec.set(vec.q(), r_coord{vec.s()});
        break;
    case r:
        vec.set(q_coord{vec.s()}, vec.r());
        break;
    case s:
        vec.set(q_coord{vec.r()}, r_coord{vec.q()});
        break;
    }
    return vec;
}

template<typename R, detail::arithmetic T>
constexpr auto to_cartesian(vector<T> vec) -> std::array<R, 2>
{
    auto const x = 3. * (vec.q().value() / 2.);
    auto const y = (std::numbers::sqrt3 / 2.) * (vec.q().value() + 2 * vec.r().value());
    return {static_cast<R>(x), static_cast<R>(y)};
}
template<typename R, detail::arithmetic T>
constexpr auto from_cartesian(std::array<T, 2> pos) -> vector<R>
{
    auto const q = (2. * pos[0]) / 3.;
    auto const r = (std::numbers::sqrt3 * pos[1] - pos[0]) / 3.;
    return vector<R>(q_coordinate<R>(q), r_coordinate<R>(r));
}

// The unit vector in +q -r direction. Has norm() == 1. Equal to -unit_rq.
inline constexpr vector<std::int8_t> unit_qr{q_coordinate<std::int8_t>(1), r_coordinate<std::int8_t>(-1)};
// The unit vector in +q -s direction. Has norm() == 1. Equal to -unit_sq.
inline constexpr vector<std::int8_t> unit_qs{q_coordinate<std::int8_t>(1), r_coordinate<std::int8_t>(0)};
// The unit vector in +r -s direction. Has norm() == 1. Equal to -unit_sr.
inline constexpr vector<std::int8_t> unit_rs{q_coordinate<std::int8_t>(0), r_coordinate<std::int8_t>(1)};
// The unit vector in +r -q direction. Has norm() == 1. Equal to -unit_qr.
inline constexpr vector<std::int8_t> unit_rq{q_coordinate<std::int8_t>(-1), r_coordinate<std::int8_t>(1)};
// The unit vector in +s -q direction. Has norm() == 1. Equal to -unit_qs.
inline constexpr vector<std::int8_t> unit_sq{q_coordinate<std::int8_t>(-1), r_coordinate<std::int8_t>(0)};
// The unit vector in +s -r direction. Has norm() == 1. Equal to -unit_rs.
inline constexpr vector<std::int8_t> unit_sr{q_coordinate<std::int8_t>(0), r_coordinate<std::int8_t>(-1)};

// The "diagonal" vector in +q direction. Has norm() == 2.
inline constexpr vector<std::int8_t> diag_q{q_coordinate<std::int8_t>(2), r_coordinate<std::int8_t>(-1)};
// The "diagonal" vector in +r direction. Has norm() == 2.
inline constexpr vector<std::int8_t> diag_r{q_coordinate<std::int8_t>(-1), r_coordinate<std::int8_t>(2)};
// The "diagonal" vector in +s direction. Has norm() == 2.
inline constexpr vector<std::int8_t> diag_s{q_coordinate<std::int8_t>(-1), r_coordinate<std::int8_t>(-1)};
} // namespace hex

#endif // HEX_VECTOR_HPP
