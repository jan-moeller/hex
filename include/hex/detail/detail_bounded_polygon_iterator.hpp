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

#ifndef DETAIL_BOUNDED_POLYGON_ITERATOR_HPP
#define DETAIL_BOUNDED_POLYGON_ITERATOR_HPP

#include "hex/coordinate.hpp"
#include "hex/vector.hpp"

#include <iterator>

#include <cstddef>

namespace hex::detail
{
template<typename T>
class bounded_polygon_iterator
{
  public:
    using value_type        = vector<T>;
    using reference         = value_type;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr bounded_polygon_iterator() = default;
    constexpr bounded_polygon_iterator(r_coordinate<T> rmin,
                                       s_coordinate<T> smin,
                                       r_coordinate<T> rmax,
                                       s_coordinate<T> smax,
                                       vector<T>       v)
        : m_rmin(rmin)
        , m_smin(smin)
        , m_rmax(rmax)
        , m_smax(smax)
        , m_v(v)
    {
    }

    constexpr auto operator++() noexcept -> bounded_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() < m_rmax && m_v.s() > m_smin)
            m_v.set(m_v.q(), m_v.r() + 1_r);
        else
        {
            m_v.set(m_v.q() + 1_q, m_rmin);
            if (m_v.s() > m_smax)
                m_v.set(m_v.q(), m_smax);
        }
        return *this;
    }
    constexpr auto operator++(int) noexcept -> bounded_polygon_iterator
    {
        auto cp = *this;
        ++(*this);
        return cp;
    }

    constexpr auto operator--() noexcept -> bounded_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() > m_rmin && m_v.s() < m_smax)
            m_v.set(m_v.q(), m_v.r() - 1_r);
        else
        {
            m_v.set(m_v.q() - 1_q, m_rmax);
            if (m_v.s() < m_smin)
                m_v.set(m_v.q(), m_smin);
        }
        return *this;
    }
    constexpr auto operator--(int) noexcept -> bounded_polygon_iterator
    {
        auto cp = *this;
        --(*this);
        return cp;
    }

    constexpr auto operator*() const noexcept -> vector<T> const& { return m_v; }

    constexpr auto operator==(bounded_polygon_iterator const&) const -> bool = default;
    constexpr auto operator<=>(bounded_polygon_iterator const&) const        = default;

  private:
    r_coordinate<T> m_rmin;
    s_coordinate<T> m_smin;
    r_coordinate<T> m_rmax;
    s_coordinate<T> m_smax;
    vector<T>       m_v;
};
} // namespace hex::detail

#endif // DETAIL_BOUNDED_POLYGON_ITERATOR_HPP
