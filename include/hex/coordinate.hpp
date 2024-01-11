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

#ifndef HEX_COORDINATE_HPP
#define HEX_COORDINATE_HPP

#include "hex/coordinate_axis.hpp"
#include "hex/detail/detail_narrowing.hpp"
#include "hex/detail/detail_parse_integer_literal.hpp"

#include <concepts>

namespace hex
{
// A hex grid coordinate along one of the 3 major axes.
template<coordinate_axis Axis, typename T = int>
    requires(std::signed_integral<T> || std::floating_point<T>)
class coordinate
{
  private:
    T m_value = 0;

  public:
    static constexpr auto axis = Axis; // The axis of this coordinate.

    // Constructs the 0 coordinate.
    constexpr coordinate() = default;

    // Constructs a coordinate from a value of the underlying type T.
    constexpr explicit coordinate(T value) noexcept;

    // Constructs a coordinate from another coordinate along the same axis, but of a different underlying type.
    // This operation is implicit if it is non-narrowing. Otherwise, an explicit cast is required.
    template<typename U>
        requires((std::signed_integral<U> || std::floating_point<U>) && !std::same_as<T, U>)
    constexpr explicit(detail::narrowing<U, T>)    //
        coordinate(coordinate<Axis, U> coordinate) // NOLINT(google-explicit-constructor)
        noexcept(detail::nonnarrowing<U, T>);

    // Constructs a coordinate from another coordinate along another axis.
    template<coordinate_axis OtherAxis>
    constexpr explicit coordinate(coordinate<OtherAxis, T> coordinate) noexcept;

    // Provides access to the underlying type.
    [[nodiscard]] constexpr auto value() const noexcept -> T const&;
    [[nodiscard]] constexpr auto value() noexcept -> T&;

    // Converts to integral or floating point type.
    template<typename U>
        requires(std::signed_integral<U> || std::floating_point<U>)
    [[nodiscard]] constexpr explicit operator U() const noexcept;

    // Compares the underlying values.
    [[nodiscard]] constexpr auto operator==(coordinate const&) const -> bool = default;
    [[nodiscard]] constexpr auto operator<=>(coordinate const&) const        = default;

    // Copies the coordinate.
    [[nodiscard]] constexpr auto operator+() const -> coordinate<Axis, decltype(+m_value)>;
    // Negates the coordinate.
    [[nodiscard]] constexpr auto operator-() const -> coordinate<Axis, decltype(-m_value)>;

    constexpr auto operator+=(coordinate const& rhs) -> coordinate&;
    constexpr auto operator-=(coordinate const& rhs) -> coordinate&;
    constexpr auto operator*=(T const& rhs) -> coordinate&;
    constexpr auto operator/=(T const& rhs) -> coordinate&;
    constexpr auto operator%=(T const& rhs) -> coordinate&
        requires requires { m_value %= rhs; };
};

// Deduction guide to resolve constructor calls to the right type.
template<coordinate_axis Axis, typename T>
coordinate(T) -> coordinate<Axis, T>;

// Sums two coordinates.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator+(coordinate<Axis, Lhs> const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} + Rhs{})>;

// Subtracts a coordinate from another.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator-(coordinate<Axis, Lhs> const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} - Rhs{})>;

// Multiplies a coordinate by a factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs)
    -> coordinate<Axis, decltype(Lhs{} * Rhs{})>;
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(Lhs const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} * Rhs{})>;

// Divides a coordinate by a factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator/(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs)
    -> coordinate<Axis, decltype(Lhs{} / Rhs{})>;
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator/(Lhs const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} / Rhs{})>;

// Computes the remainder of a coordinate and factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator%(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs)
    -> coordinate<Axis, decltype(Lhs{} % Rhs{})>
    requires requires { Lhs{} % Rhs{}; };

// Cast the coordinate to the given underlying type T.
template<typename T, coordinate_axis Axis, typename U>
    requires(std::signed_integral<T> || std::floating_point<T>)
[[nodiscard]] constexpr auto coordinate_cast(coordinate<Axis, U> const& coord) -> coordinate<Axis, T>;

template<typename T = int>
using q_coordinate = coordinate<coordinate_axis::q, T>;
template<typename T = int>
using r_coordinate = coordinate<coordinate_axis::r, T>;
template<typename T = int>
using s_coordinate = coordinate<coordinate_axis::s, T>;

namespace literals
{
// Parses an integer literal as a q coordinate. The underlying type is chosen as the shortest type that can represent
// the given value from the following list: int, long int, long long int.
template<char... Digits>
[[nodiscard]] constexpr auto operator""_q() noexcept;

// Parses an integer literal as an r coordinate. The underlying type is chosen as the shortest type that can represent
// the given value from the following list: int, long int, long long int.
template<char... Digits>
[[nodiscard]] constexpr auto operator""_r() noexcept;

// Parses an integer literal as an s coordinate. The underlying type is chosen as the shortest type that can represent
// the given value from the following list: int, long int, long long int.
template<char... Digits>
[[nodiscard]] constexpr auto operator""_s() noexcept;
} // namespace literals

// ------------------------------ implementation below ------------------------------

template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr coordinate<Axis, T>::coordinate(T value) noexcept
    : m_value(value)
{
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
template<typename U>
    requires((std::signed_integral<U> || std::floating_point<U>) && !std::same_as<T, U>)
constexpr coordinate<Axis, T>::coordinate(coordinate<Axis, U> coordinate) noexcept(detail::nonnarrowing<U, T>)
    : m_value(coordinate.value())
{
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
template<coordinate_axis OtherAxis>
constexpr coordinate<Axis, T>::coordinate(coordinate<OtherAxis, T> coordinate) noexcept
    : m_value(coordinate.value())
{
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::value() const noexcept -> T const&
{
    return m_value;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::value() noexcept -> T&
{
    return m_value;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
template<typename U>
    requires(std::signed_integral<U> || std::floating_point<U>)
constexpr coordinate<Axis, T>::operator U() const noexcept
{
    return U(m_value);
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator+() const -> coordinate<Axis, decltype(+m_value)>
{
    return coordinate<Axis, decltype(+m_value)>{+m_value};
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator-() const -> coordinate<Axis, decltype(-m_value)>
{
    return coordinate<Axis, decltype(-m_value)>{-m_value};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator+(coordinate<Axis, Lhs> const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} + Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} + Rhs{})>{lhs.value() + rhs.value()};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator-(coordinate<Axis, Lhs> const& lhs, coordinate<Axis, Rhs> const& rhs)
    -> coordinate<Axis, decltype(Lhs{} - Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} - Rhs{})>{lhs.value() - rhs.value()};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator*(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs) -> coordinate<Axis, decltype(Lhs{} * Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} * Rhs{})>{lhs.value() * rhs};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator*(Lhs const& lhs, coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} * Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} * Rhs{})>{lhs * rhs.value()};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator/(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs) -> coordinate<Axis, decltype(Lhs{} / Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} / Rhs{})>{lhs.value() / rhs};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator%(coordinate<Axis, Lhs> const& lhs, Rhs const& rhs) -> coordinate<Axis, decltype(Lhs{} % Rhs{})>
    requires requires { Lhs{} % Rhs{}; }
{
    return coordinate<Axis, decltype(Lhs{} % Rhs{})>{lhs.value() % rhs};
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator+=(coordinate const& rhs) -> coordinate&
{
    m_value += rhs.m_value;
    return *this;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator-=(coordinate const& rhs) -> coordinate&
{
    m_value -= rhs.m_value;
    return *this;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator*=(T const& rhs) -> coordinate&
{
    m_value *= rhs;
    return *this;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator/=(T const& rhs) -> coordinate&
{
    m_value /= rhs;
    return *this;
}
template<coordinate_axis Axis, typename T>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate<Axis, T>::operator%=(T const& rhs) -> coordinate&
    requires requires { m_value %= rhs; }
{
    m_value %= rhs;
    return *this;
}

template<typename T, coordinate_axis Axis, typename U>
    requires(std::signed_integral<T> || std::floating_point<T>)
constexpr auto coordinate_cast(coordinate<Axis, U> const& coord) -> coordinate<Axis, T>
{
    return coordinate<Axis, T>(static_cast<T>(coord.value()));
}

namespace literals
{
template<char... Digits>
constexpr auto operator""_q() noexcept
{
    static constexpr auto num = detail::parse_integer_literal<Digits...>();
    return q_coordinate{num};
}
template<char... Digits>
constexpr auto operator""_r() noexcept
{
    static constexpr auto num = detail::parse_integer_literal<Digits...>();
    return r_coordinate{num};
}
template<char... Digits>
constexpr auto operator""_s() noexcept
{
    static constexpr auto num = detail::parse_integer_literal<Digits...>();
    return s_coordinate{num};
}
} // namespace literals
} // namespace hex

#endif // HEX_COORDINATE_HPP
