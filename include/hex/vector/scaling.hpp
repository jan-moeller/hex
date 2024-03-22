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

#ifndef HEX_SCALING_HPP
#define HEX_SCALING_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/coordinate.hpp"
#include "hex/vector/coordinate_axis.hpp"

#include <concepts>
#include <variant>

namespace hex
{
// A helper type holding information about a scaling transformation.
template<detail::arithmetic T>
class scaling
{
  public:
    // Creates a uniform scaling by some factor.
    constexpr explicit scaling(T factor)
        : m_factor(factor)
    {
    }
    // Creates a scaling in q direction.
    constexpr explicit scaling(q_coordinate<T> factor) noexcept
        requires std::floating_point<T>
        : m_factor(factor)
    {
    }
    // Creates a scaling in r direction.
    constexpr explicit scaling(r_coordinate<T> factor) noexcept
        requires std::floating_point<T>
        : m_factor(factor)
    {
    }
    // Creates a scaling in s direction.
    constexpr explicit scaling(s_coordinate<T> factor) noexcept
        requires std::floating_point<T>
        : m_factor(factor)
    {
    }

    // Retrieves the scaling information.
    [[nodiscard]] constexpr auto
    factor() const noexcept -> std::variant<T, q_coordinate<T>, r_coordinate<T>, s_coordinate<T>> const&
    {
        return m_factor;
    }

  private:
    std::variant<T, q_coordinate<T>, r_coordinate<T>, s_coordinate<T>> m_factor;
};
template<detail::arithmetic T>
scaling(T) -> scaling<T>;
template<coordinate_axis Axis, detail::arithmetic T>
scaling(coordinate<Axis, T>) -> scaling<T>;
} // namespace hex

#endif // HEX_SCALING_HPP
