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

#ifndef CONVEX_GRID_SHAPE_HPP
#define CONVEX_GRID_SHAPE_HPP

#include "hex/convex_polygon_parameters.hpp"
#include "hex/convex_polygon_view.hpp"
#include "hex/detail/detail_arithmetic.hpp"
#include "hex/detail/detail_hexagon_size.hpp"
#include "hex/vector.hpp"

#include <cstddef>

namespace hex
{
// Shape type to be used with grid<>. Can represent grids of any convex shape.
template<detail::arithmetic T>
class convex_grid_shape : public convex_polygon_view<T>
{
  public:
    constexpr explicit convex_grid_shape(convex_polygon_parameters<T> params);

    [[nodiscard]] constexpr auto index_of(vector<T> v) const -> std::size_t;
};

// ------------------------------ implementation below ------------------------------

template<detail::arithmetic T>
constexpr convex_grid_shape<T>::convex_grid_shape(convex_polygon_parameters<T> params)
    : convex_polygon_view<T>(std::move(params))
{
}

template<detail::arithmetic T>
constexpr auto convex_grid_shape<T>::index_of(vector<T> v) const -> std::size_t
{
    return (*this)[v];
}

} // namespace hex

#endif // CONVEX_GRID_SHAPE_HPP
