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

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/coordinate.hpp"
#include "hex/vector/vector.hpp"
#include "hex/views/convex_polygon/convex_polygon_parameters.hpp"
#include "hex/views/convex_polygon/detail/detail_hexagon_size.hpp"

#include <compare>
#include <iterator>

#include <cassert>
#include <cstddef>

namespace hex
{
template<detail::arithmetic T>
class convex_polygon_view;

namespace detail
{
template<typename T>
class convex_polygon_iterator
{
  public:
    using value_type        = vector<T> const;
    using difference_type   = std::ptrdiff_t;
    using iterator_concept  = std::random_access_iterator_tag;
    using iterator_category = std::input_iterator_tag;

    constexpr convex_polygon_iterator() = default;
    constexpr convex_polygon_iterator(convex_polygon_parameters<T> params, std::size_t idx)
        : convex_polygon_iterator(params, from_index(idx, params), idx)
    {
    }
    constexpr convex_polygon_iterator(convex_polygon_parameters<T> params, vector<T> v)
        : convex_polygon_iterator(params, v, to_index(v, params))
    {
    }

    constexpr auto operator++() noexcept -> convex_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() < m_params.rmax() && m_v.s() > m_params.smin())
            m_v.set(m_v.q(), m_v.r() + 1_r);
        else
        {
            m_v.set(m_v.q() + 1_q, m_params.rmin());
            if (m_v.s() > m_params.smax())
                m_v.set(m_v.q(), m_params.smax());
        }
        ++m_idx;
        return *this;
    }
    constexpr auto operator++(int) noexcept -> convex_polygon_iterator
    {
        auto cp = *this;
        ++(*this);
        return cp;
    }
    friend constexpr auto operator+(convex_polygon_iterator const& iter,
                                    difference_type                n) noexcept -> convex_polygon_iterator
    {
        return convex_polygon_iterator{iter.m_params, iter.m_idx + n};
    }
    friend constexpr auto operator+(difference_type                n,
                                    convex_polygon_iterator const& iter) noexcept -> convex_polygon_iterator
    {
        return iter + n;
    }
    constexpr auto operator+=(difference_type n) noexcept -> convex_polygon_iterator&
    {
        m_idx += n;
        m_v = from_index(m_idx, m_params);
        return *this;
    }

    constexpr auto operator--() noexcept -> convex_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() > m_params.rmin() && m_v.s() < m_params.smax())
            m_v.set(m_v.q(), m_v.r() - 1_r);
        else
        {
            m_v.set(m_v.q() - 1_q, m_params.rmax());
            if (m_v.s() < m_params.smin())
                m_v.set(m_v.q(), m_params.smin());
        }
        --m_idx;
        return *this;
    }
    constexpr auto operator--(int) noexcept -> convex_polygon_iterator
    {
        auto cp = *this;
        --(*this);
        return cp;
    }
    friend constexpr auto operator-(convex_polygon_iterator const& iter,
                                    difference_type                n) noexcept -> convex_polygon_iterator
    {
        return convex_polygon_iterator{iter.m_params, iter.m_idx - n};
    }
    friend constexpr auto operator-(convex_polygon_iterator const& lhs,
                                    convex_polygon_iterator const& rhs) noexcept -> difference_type
    {
        return lhs.m_idx - rhs.m_idx;
    }
    constexpr auto operator-=(difference_type n) noexcept -> convex_polygon_iterator&
    {
        m_idx -= n;
        m_v = from_index(m_idx, m_params);
        return *this;
    }

    constexpr auto operator*() const noexcept -> vector<T> const // NOLINT(readability-const-return-type)
    {
        return m_v;
    }

    // NOLINTNEXTLINE(readability-const-return-type)
    constexpr auto operator[](difference_type n) const -> value_type { return *(*this + n); }

    friend constexpr auto operator==(convex_polygon_iterator const& lhs, convex_polygon_iterator const& rhs) -> bool
    {
        return lhs.m_idx == rhs.m_idx;
    }
    friend constexpr auto operator<=>(convex_polygon_iterator const& lhs,
                                      convex_polygon_iterator const& rhs) -> std::strong_ordering
    {
        if (lhs == rhs)
            return std::strong_ordering::equal;
        return lhs.m_idx < rhs.m_idx ? std::strong_ordering::less : std::strong_ordering::greater;
    }

  private:
    constexpr auto from_index(std::size_t idx, convex_polygon_parameters<T> const& params) const noexcept -> vector<T>
    {
        auto const [q, r] = detail::index_to_qr(idx,
                                                params.qmin().value(),
                                                params.rmin().value(),
                                                params.smin().value(),
                                                params.rmax().value(),
                                                params.smax().value());
        return vector<T>{q_coordinate<T>{static_cast<T>(q)}, r_coordinate<T>{static_cast<T>(r)}};
    }
    constexpr auto to_index(vector<T> const&                    v,
                            convex_polygon_parameters<T> const& params) const noexcept -> std::size_t
    {
        return detail::qr_to_index(v.q().value(),
                                   v.r().value(),
                                   params.qmin().value(),
                                   params.rmin().value(),
                                   params.smin().value(),
                                   params.rmax().value(),
                                   params.smax().value());
    }
    constexpr convex_polygon_iterator(convex_polygon_parameters<T> params, vector<T> v, std::size_t idx)
        : m_params(params)
        , m_v(v)
        , m_idx(idx)
    {
    }

    convex_polygon_parameters<T> m_params{q_coordinate<T>{0},
                                          r_coordinate<T>{0},
                                          s_coordinate<T>{0},
                                          q_coordinate<T>{0},
                                          r_coordinate<T>{0},
                                          s_coordinate<T>{0}};
    vector<T>                    m_v;
    std::size_t                  m_idx = 0;

    friend class convex_polygon_view<T>;
};
} // namespace detail
} // namespace hex

#endif // DETAIL_BOUNDED_POLYGON_ITERATOR_HPP
