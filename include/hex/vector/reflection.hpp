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

#ifndef HEX_REFLECTION_HPP
#define HEX_REFLECTION_HPP

#include "hex/vector/coordinate_axis.hpp"

namespace hex
{
// A helper type holding information about a reflection transformation.
class reflection
{
  public:
    // Creates a reflection across an axis.
    constexpr explicit reflection(coordinate_axis axis) noexcept
        : m_axis(axis)
    {
    }

    // Retrieves the reflection axis.
    [[nodiscard]] constexpr auto axis() const noexcept -> coordinate_axis const& { return m_axis; }

  private:
    coordinate_axis m_axis;
};
} // namespace hex

#endif // HEX_REFLECTION_HPP
