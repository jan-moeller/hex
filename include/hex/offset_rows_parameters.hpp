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

#ifndef HEX_OFFSET_ROWS_PARAMETERS_HPP
#define HEX_OFFSET_ROWS_PARAMETERS_HPP

#include "hex/coordinate_axis.hpp"
#include "hex/offset_parity.hpp"
#include "hex/vector.hpp"

#include <concepts>

#include <cstddef>

namespace hex
{
// Aggregates all parameters required to describe a semi-rectangular region on a hex-grid. Since exact rectangular
// regions are not possible, the parameters describe a region where every other row is shifted ("offset") by half a hex
// to the same side.
// The width and height determine the size of the region - it starts at corner, and extends by width along axis in
// positive direction, and by height into the orthogonal direction (which is between axis and the next axis in q, r, s
// order). The parity determines whether even or odd rows are offset along axis.
template<std::integral T>
class offset_rows_parameters
{
  public:
    constexpr offset_rows_parameters(std::size_t     width,
                                     std::size_t     height,
                                     coordinate_axis axis   = coordinate_axis::q,
                                     offset_parity   parity = offset_parity::odd,
                                     vector<T>       corner = {});

    [[nodiscard]] constexpr auto width() const noexcept -> std::size_t;
    [[nodiscard]] constexpr auto height() const noexcept -> std::size_t;
    [[nodiscard]] constexpr auto axis() const noexcept -> coordinate_axis;
    [[nodiscard]] constexpr auto parity() const noexcept -> offset_parity;
    [[nodiscard]] constexpr auto corner() const noexcept -> vector<T>;

    [[nodiscard]] constexpr auto operator==(offset_rows_parameters const& other) const noexcept -> bool = default;

  private:
    std::size_t     m_width;
    std::size_t     m_height;
    coordinate_axis m_axis   = coordinate_axis::q;
    offset_parity   m_parity = offset_parity::odd;
    vector<T>       m_corner = {};
};
} // namespace hex

// ------------------------------ implementation below ------------------------------

template<std::integral T> // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr hex::offset_rows_parameters<T>::offset_rows_parameters(std::size_t          width,
                                                                 std::size_t          height,
                                                                 hex::coordinate_axis axis,
                                                                 hex::offset_parity   parity,
                                                                 hex::vector<T>       corner)
    : m_width(width)
    , m_height(height)
    , m_axis(axis)
    , m_parity(parity)
    , m_corner(corner)
{
}

template<std::integral T>
constexpr auto hex::offset_rows_parameters<T>::width() const noexcept -> std::size_t
{
    return m_width;
}
template<std::integral T>
constexpr auto hex::offset_rows_parameters<T>::height() const noexcept -> std::size_t
{
    return m_height;
}
template<std::integral T>
constexpr auto hex::offset_rows_parameters<T>::axis() const noexcept -> hex::coordinate_axis
{
    return m_axis;
}
template<std::integral T>
constexpr auto hex::offset_rows_parameters<T>::parity() const noexcept -> hex::offset_parity
{
    return m_parity;
}
template<std::integral T>
constexpr auto hex::offset_rows_parameters<T>::corner() const noexcept -> hex::vector<T>
{
    return m_corner;
}

#endif // HEX_OFFSET_ROWS_PARAMETERS_HPP
