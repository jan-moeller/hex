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

#ifndef HEX_DETAIL_GRID_ITERATOR_HPP
#define HEX_DETAIL_GRID_ITERATOR_HPP

#include <iterator>
#include <ranges>
#include <type_traits>

#include <cstddef>

namespace hex::detail
{
template<class Grid, bool Const>
class grid_iterator
{
  public:
    using value_type        = typename Grid::value_type;
    using reference         = std::conditional_t<Const, typename Grid::const_reference, typename Grid::reference>;
    using difference_type   = std::ptrdiff_t;
    using iterator_concept  = std::bidirectional_iterator_tag;
    using iterator_category = std::input_iterator_tag;

    constexpr grid_iterator() = default;

    template<bool WasConst>
        requires(Const && !WasConst)
    constexpr grid_iterator(grid_iterator<Grid, WasConst> const& rhs)
        : m_grid(rhs.m_grid)
        , m_iter(rhs.m_iter)
    {
    }

    constexpr auto operator++() noexcept -> grid_iterator&
    {
        ++m_iter;
        return *this;
    }
    constexpr auto operator++(int) noexcept -> grid_iterator
    {
        auto cp = *this;
        ++(*this);
        return cp;
    }

    constexpr auto operator--() noexcept -> grid_iterator&
    {
        --m_iter;
        return *this;
    }
    constexpr auto operator--(int) noexcept -> grid_iterator
    {
        auto cp = *this;
        --(*this);
        return cp;
    }

    constexpr auto operator*() const noexcept -> reference { return {*m_iter, (*m_grid)[*m_iter]}; }

    constexpr auto operator==(grid_iterator const& other) const -> bool
    {
        return !m_grid || !other.m_grid || (m_grid == other.m_grid && m_iter == other.m_iter);
    }

  private:
    using GridPtr  = std::conditional_t<Const, Grid const*, Grid*>;
    using BaseIter = std::ranges::const_iterator_t<typename Grid::shape_type>;

    GridPtr  m_grid = nullptr;
    BaseIter m_iter;

    constexpr grid_iterator(GridPtr grid, std::ranges::const_iterator_t<typename Grid::shape_type> iter)
        : m_grid(grid)
        , m_iter(iter)
    {
    }

    friend Grid;
};
} // namespace hex::detail

#endif // HEX_DETAIL_GRID_ITERATOR_HPP
