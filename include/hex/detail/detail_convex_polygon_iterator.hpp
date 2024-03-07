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

#include "hex/convex_polygon_parameters.hpp"
#include "hex/vector.hpp"

#include <iterator>
#include <optional>

#include <cstddef>

namespace hex::detail
{
template<typename T>
class convex_polygon_iterator
{
  public:
    using value_type        = vector<T>;
    using reference         = value_type const;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr convex_polygon_iterator() = default;
    constexpr convex_polygon_iterator(convex_polygon_parameters<T> params, vector<T> v)
        : m_params(params)
        , m_v(v)
    {
    }

    constexpr auto operator++() noexcept -> convex_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() < m_params->rmax() && m_v.s() > m_params->smin())
            m_v.set(m_v.q(), m_v.r() + 1_r);
        else
        {
            m_v.set(m_v.q() + 1_q, m_params->rmin());
            if (m_v.s() > m_params->smax())
                m_v.set(m_v.q(), m_params->smax());
        }
        return *this;
    }
    constexpr auto operator++(int) noexcept -> convex_polygon_iterator
    {
        auto cp = *this;
        ++(*this);
        return cp;
    }

    constexpr auto operator--() noexcept -> convex_polygon_iterator&
    {
        using namespace literals;
        if (m_v.r() > m_params->rmin() && m_v.s() < m_params->smax())
            m_v.set(m_v.q(), m_v.r() - 1_r);
        else
        {
            m_v.set(m_v.q() - 1_q, m_params->rmax());
            if (m_v.s() < m_params->smin())
                m_v.set(m_v.q(), m_params->smin());
        }
        return *this;
    }
    constexpr auto operator--(int) noexcept -> convex_polygon_iterator
    {
        auto cp = *this;
        --(*this);
        return cp;
    }

    constexpr auto operator*() const noexcept -> reference { return m_v; } // NOLINT(readability-const-return-type)

    constexpr auto operator==(convex_polygon_iterator const& other) const -> bool
    {
        return (!m_params.has_value() && !other.m_params.has_value())
               || (!m_params.has_value() && !other.m_params->contains(other.m_v))
               || (!other.m_params.has_value() && !m_params->contains(m_v))
               || (m_params == other.m_params && m_v == other.m_v);
    }

  private:
    std::optional<convex_polygon_parameters<T>> m_params;
    vector<T>                                   m_v;
};
} // namespace hex::detail

#endif // DETAIL_BOUNDED_POLYGON_ITERATOR_HPP
