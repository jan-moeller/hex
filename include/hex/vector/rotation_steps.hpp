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

#ifndef HEX_ROTATION_STEPS_HPP
#define HEX_ROTATION_STEPS_HPP

#include <compare>

#include <cstdint>

namespace hex
{
// A helper type designating 60° rotation steps.
class rotation_steps
{
  public:
    // Constructs a rotation_steps object with no rotation.
    constexpr rotation_steps() = default;

    // Constructs a rotation_steps object with the given number of rotations. If amount is positive, then rotations are
    // in clockwise direction. If amount is negative, then rotations are in counter-clockwise direction.
    constexpr explicit rotation_steps(std::int8_t amount) noexcept;

    // Returns the number of rotations in clockwise direction in domain [0, 5].
    [[nodiscard]] constexpr auto clockwise_steps() const noexcept -> std::uint8_t;

    // Returns the number of rotations in counter-clockwise direction in domain [0, 5].
    [[nodiscard]] constexpr auto counter_clockwise_steps() const noexcept -> std::uint8_t;

    // Compares the underlying values.
    [[nodiscard]] constexpr auto operator==(rotation_steps const&) const -> bool = default;
    [[nodiscard]] constexpr auto operator<=>(rotation_steps const&) const        = default;

    // Copies the coordinate.
    [[nodiscard]] constexpr auto operator+() const -> rotation_steps;
    // Negates the coordinate.
    [[nodiscard]] constexpr auto operator-() const -> rotation_steps;

    constexpr auto operator+=(rotation_steps const& rhs) -> rotation_steps&;
    constexpr auto operator-=(rotation_steps const& rhs) -> rotation_steps&;

  private:
    std::uint8_t m_rotations = 0;
};

// Sums two rotation_steps.
[[nodiscard]] constexpr auto operator+(rotation_steps const& lhs, rotation_steps const& rhs) -> rotation_steps;

// Subtracts a rotation_steps from another.
[[nodiscard]] constexpr auto operator-(rotation_steps const& lhs, rotation_steps const& rhs) -> rotation_steps;

// ------------------------------ implementation below ------------------------------

constexpr rotation_steps::rotation_steps(std::int8_t amount) noexcept
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    : m_rotations(amount % 6 + static_cast<int>((amount % 6) < 0) * 6)
{
}
constexpr auto rotation_steps::clockwise_steps() const noexcept -> std::uint8_t
{
    return m_rotations;
}
constexpr auto rotation_steps::counter_clockwise_steps() const noexcept -> std::uint8_t
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    return (6 - m_rotations) % 6;
}

constexpr auto rotation_steps::operator+() const -> rotation_steps
{
    return *this;
}
constexpr auto rotation_steps::operator-() const -> rotation_steps
{
    return rotation_steps(static_cast<std::int8_t>(-m_rotations));
}
constexpr auto rotation_steps::operator+=(rotation_steps const& rhs) -> rotation_steps&
{
    m_rotations += rhs.m_rotations;
    return *this;
}
constexpr auto rotation_steps::operator-=(rotation_steps const& rhs) -> rotation_steps&
{
    m_rotations -= rhs.m_rotations;
    return *this;
}
constexpr auto operator+(rotation_steps const& lhs, rotation_steps const& rhs) -> rotation_steps
{
    return rotation_steps(static_cast<std::int8_t>(lhs.clockwise_steps() + rhs.clockwise_steps()));
}
constexpr auto operator-(rotation_steps const& lhs, rotation_steps const& rhs) -> rotation_steps
{
    return rotation_steps(static_cast<std::int8_t>(lhs.clockwise_steps() - rhs.clockwise_steps()));
}

inline constexpr rotation_steps rot_0       = rotation_steps(0);
inline constexpr rotation_steps rot_60_cw   = rotation_steps(1);
inline constexpr rotation_steps rot_120_cw  = rotation_steps(2);
inline constexpr rotation_steps rot_180_cw  = rotation_steps(3);
inline constexpr rotation_steps rot_240_cw  = rotation_steps(4);
inline constexpr rotation_steps rot_300_cw  = rotation_steps(5);
inline constexpr rotation_steps rot_60_ccw  = rotation_steps(-1);
inline constexpr rotation_steps rot_120_ccw = rotation_steps(-2);
inline constexpr rotation_steps rot_180_ccw = rotation_steps(-3);
inline constexpr rotation_steps rot_240_ccw = rotation_steps(-4);
inline constexpr rotation_steps rot_300_ccw = rotation_steps(-5);
} // namespace hex

#endif // HEX_ROTATION_STEPS_HPP
