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

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_narrowing.hpp"
#include "hex/detail/detail_parse_integer_literal.hpp"
#include "hex/vector/coordinate_axis.hpp"

#include <format>
#include <limits>

namespace hex
{
// A hex grid coordinate along one of the 3 major axes. Coordinates must not exceed the range [min_value, max_value].
template<coordinate_axis Axis, detail::arithmetic T = int>
class coordinate
{
  private:
    T m_value = 0;

  public:
    static constexpr auto axis      = Axis;                              // The axis of this coordinate.
    static constexpr auto max_value = std::numeric_limits<T>::max() / 2; // The greatest valid coordinate value
    static constexpr auto min_value = -max_value;                        // The lowest valid coordinate value

    // Constructs the 0 coordinate.
    constexpr coordinate() = default;

    // Constructs a coordinate from a value of the underlying type T.
    constexpr explicit coordinate(T value) noexcept;

    // Constructs a coordinate from another coordinate along the same axis, but of a different underlying type.
    // This operation is implicit if it is non-narrowing. Otherwise, an explicit cast is required.
    template<detail::arithmetic U>
    constexpr explicit(detail::narrowing<U, T>)
        coordinate(coordinate<Axis, U> coordinate) // NOLINT(google-explicit-constructor)
        noexcept(detail::nonnarrowing<U, T>);

    // Constructs a coordinate from another coordinate along another axis.
    template<coordinate_axis OtherAxis>
    constexpr explicit coordinate(coordinate<OtherAxis, T> coordinate) noexcept;

    // Provides access to the underlying type.
    [[nodiscard]] constexpr auto value() const noexcept -> T const&;
    [[nodiscard]] constexpr auto value() noexcept -> T&;

    // Converts to integral or floating point type.
    template<detail::arithmetic U>
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
[[nodiscard]] constexpr auto operator+(coordinate<Axis, Lhs> const& lhs,
                                       coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} + Rhs{})>;

// Subtracts a coordinate from another.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator-(coordinate<Axis, Lhs> const& lhs,
                                       coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} - Rhs{})>;

// Multiplies a coordinate by a factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(coordinate<Axis, Lhs> const& lhs,
                                       Rhs const&                   rhs) -> coordinate<Axis, decltype(Lhs{} * Rhs{})>;
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator*(Lhs const&                   lhs,
                                       coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} * Rhs{})>;

// Divides a coordinate by a factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator/(coordinate<Axis, Lhs> const& lhs,
                                       Rhs const&                   rhs) -> coordinate<Axis, decltype(Lhs{} / Rhs{})>;
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator/(Lhs const&                   lhs,
                                       coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} / Rhs{})>;

// Computes the remainder of a coordinate and factor.
template<coordinate_axis Axis, typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto operator%(coordinate<Axis, Lhs> const& lhs,
                                       Rhs const&                   rhs) -> coordinate<Axis, decltype(Lhs{} % Rhs{})>
    requires requires { Lhs{} % Rhs{}; };

// Cast the coordinate to the given underlying type T.
template<detail::arithmetic T, coordinate_axis Axis, typename U>
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

// Floating point literal of a q coordinate of underlying type float.
[[nodiscard]] constexpr auto operator""_qf(long double) noexcept -> q_coordinate<float>;
// Floating point literal of a q coordinate of underlying type double.
[[nodiscard]] constexpr auto operator""_q(long double) noexcept -> q_coordinate<double>;
// Floating point literal of a q coordinate of underlying type long double.
[[nodiscard]] constexpr auto operator""_ql(long double) noexcept -> q_coordinate<long double>;

// Floating point literal of a r coordinate of underlying type float.
[[nodiscard]] constexpr auto operator""_rf(long double) noexcept -> r_coordinate<float>;
// Floating point literal of a r coordinate of underlying type double.
[[nodiscard]] constexpr auto operator""_r(long double) noexcept -> r_coordinate<double>;
// Floating point literal of a r coordinate of underlying type long double.
[[nodiscard]] constexpr auto operator""_rl(long double) noexcept -> r_coordinate<long double>;

// Floating point literal of a s coordinate of underlying type float.
[[nodiscard]] constexpr auto operator""_sf(long double) noexcept -> s_coordinate<float>;
// Floating point literal of a s coordinate of underlying type double.
[[nodiscard]] constexpr auto operator""_s(long double) noexcept -> s_coordinate<double>;
// Floating point literal of a s coordinate of underlying type long double.
[[nodiscard]] constexpr auto operator""_sl(long double) noexcept -> s_coordinate<long double>;
} // namespace literals
} // namespace hex

template<hex::coordinate_axis Axis, typename T, typename CharT>
struct std::formatter<hex::coordinate<Axis, T>, CharT>
{
    std::formatter<T, CharT> formatter;

    template<class ParseContext>
    constexpr auto parse(ParseContext& ctx) -> ParseContext::iterator;

    template<class FmtContext>
    constexpr auto format(hex::coordinate<Axis, T> coord, FmtContext& ctx) const -> FmtContext::iterator;
};

// ------------------------------ implementation below ------------------------------

namespace hex
{
template<coordinate_axis Axis, detail::arithmetic T>
constexpr coordinate<Axis, T>::coordinate(T value) noexcept
    : m_value(value)
{
}
template<coordinate_axis Axis, detail::arithmetic T>
template<detail::arithmetic U>
constexpr coordinate<Axis, T>::coordinate(coordinate<Axis, U> coordinate) noexcept(detail::nonnarrowing<U, T>)
    : m_value(coordinate.value())
{
}
template<coordinate_axis Axis, detail::arithmetic T>
template<coordinate_axis OtherAxis>
constexpr coordinate<Axis, T>::coordinate(coordinate<OtherAxis, T> coordinate) noexcept
    : m_value(coordinate.value())
{
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::value() const noexcept -> T const&
{
    return m_value;
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::value() noexcept -> T&
{
    return m_value;
}
template<coordinate_axis Axis, detail::arithmetic T>
template<detail::arithmetic U>
constexpr coordinate<Axis, T>::operator U() const noexcept
{
    return U(m_value);
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator+() const -> coordinate<Axis, decltype(+m_value)>
{
    return coordinate<Axis, decltype(+m_value)>{+m_value};
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator-() const -> coordinate<Axis, decltype(-m_value)>
{
    return coordinate<Axis, decltype(-m_value)>{-m_value};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator+(coordinate<Axis, Lhs> const& lhs,
                         coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} + Rhs{})>
{
    return coordinate<Axis, decltype(Lhs{} + Rhs{})>{lhs.value() + rhs.value()};
}
template<coordinate_axis Axis, typename Lhs, typename Rhs>
constexpr auto operator-(coordinate<Axis, Lhs> const& lhs,
                         coordinate<Axis, Rhs> const& rhs) -> coordinate<Axis, decltype(Lhs{} - Rhs{})>
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
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator+=(coordinate const& rhs) -> coordinate&
{
    m_value += rhs.m_value;
    return *this;
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator-=(coordinate const& rhs) -> coordinate&
{
    m_value -= rhs.m_value;
    return *this;
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator*=(T const& rhs) -> coordinate&
{
    m_value *= rhs;
    return *this;
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator/=(T const& rhs) -> coordinate&
{
    m_value /= rhs;
    return *this;
}
template<coordinate_axis Axis, detail::arithmetic T>
constexpr auto coordinate<Axis, T>::operator%=(T const& rhs) -> coordinate&
    requires requires { m_value %= rhs; }
{
    m_value %= rhs;
    return *this;
}

template<detail::arithmetic T, coordinate_axis Axis, typename U>
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

constexpr auto operator""_qf(long double n) noexcept -> q_coordinate<float>
{
    return q_coordinate<float>(n);
}
constexpr auto operator""_q(long double n) noexcept -> q_coordinate<double>
{
    return q_coordinate<double>(n);
}
constexpr auto operator""_ql(long double n) noexcept -> q_coordinate<long double>
{
    return q_coordinate<long double>(n);
}
constexpr auto operator""_rf(long double n) noexcept -> r_coordinate<float>
{
    return r_coordinate<float>(n);
}
constexpr auto operator""_r(long double n) noexcept -> r_coordinate<double>
{
    return r_coordinate<double>(n);
}
constexpr auto operator""_rl(long double n) noexcept -> r_coordinate<long double>
{
    return r_coordinate<long double>(n);
}
constexpr auto operator""_sf(long double n) noexcept -> s_coordinate<float>
{
    return s_coordinate<float>(n);
}
constexpr auto operator""_s(long double n) noexcept -> s_coordinate<double>
{
    return s_coordinate<double>(n);
}
constexpr auto operator""_sl(long double n) noexcept -> s_coordinate<long double>
{
    return s_coordinate<long double>(n);
}
} // namespace literals
} // namespace hex

template<hex::coordinate_axis Axis, typename T, typename CharT>
template<class ParseContext>
constexpr auto std::formatter<hex::coordinate<Axis, T>, CharT>::parse(ParseContext& ctx) -> ParseContext::iterator
{
    return formatter.parse(ctx);
}

template<hex::coordinate_axis Axis, typename T, typename CharT>
template<class FmtContext>
constexpr auto std::formatter<hex::coordinate<Axis, T>, CharT>::format(hex::coordinate<Axis, T> coord,
                                                                       FmtContext& ctx) const -> FmtContext::iterator
{
    return formatter.format(coord.value(), ctx);
}

#endif // HEX_COORDINATE_HPP