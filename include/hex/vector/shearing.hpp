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

#ifndef HEX_SHEARING_HPP
#define HEX_SHEARING_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/coordinate.hpp"
#include "hex/vector/coordinate_axis.hpp"

#include <variant>

namespace hex
{
// A helper type holding information about a shearing transformation.
template<detail::arithmetic T>
class shearing
{
  public:
    // Create a shearing along the q coordinate.
    constexpr explicit shearing(q_coordinate<T> strain) noexcept
        : m_strain(strain)
    {
    }
    // Create a shearing along the r coordinate.
    constexpr explicit shearing(r_coordinate<T> strain) noexcept
        : m_strain(strain)
    {
    }
    // Create a shearing along the s coordinate.
    constexpr explicit shearing(s_coordinate<T> strain) noexcept
        : m_strain(strain)
    {
    }

    // Retrieve the shear information.
    [[nodiscard]] constexpr auto
    strain() const noexcept -> std::variant<q_coordinate<T>, r_coordinate<T>, s_coordinate<T>> const&
    {
        return m_strain;
    }

  private:
    std::variant<q_coordinate<T>, r_coordinate<T>, s_coordinate<T>> m_strain;
};
template<coordinate_axis Axis, detail::arithmetic T>
shearing(coordinate<Axis, T>) -> shearing<T>;
} // namespace hex

#endif // HEX_SHEARING_HPP
