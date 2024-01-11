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

#ifndef HEX_DETAIL_VECTOR_ITERATOR_HPP
#define HEX_DETAIL_VECTOR_ITERATOR_HPP

#include "hex/coordinate_axis.hpp"

#include <array>
#include <compare>
#include <utility>

#include <cstdint>

namespace hex::detail
{
template<class Vector>
class vector_iterator
{
  public:
    using value_type      = std::pair<coordinate_axis const, typename Vector::mapped_type>;
    using difference_type = std::int8_t;

    constexpr vector_iterator() = default;

    constexpr auto operator*() const -> value_type { return {coordinate_axis(m_axis), s_mapping[m_axis](*m_vector)}; }

    constexpr auto operator++() -> vector_iterator&
    {
        ++m_axis;
        return *this;
    }

    constexpr auto operator++(int) -> vector_iterator { return ++auto(*this); }

    constexpr auto operator+=(difference_type n) -> vector_iterator&
    {
        m_axis += n;
        return *this;
    }

    friend constexpr auto operator+(vector_iterator const& v, difference_type n) -> vector_iterator
    {
        return auto(v) += n;
    }
    friend constexpr auto operator+(difference_type n, vector_iterator const& v) -> vector_iterator
    {
        return auto(v) += n;
    }

    constexpr auto operator--() -> vector_iterator&
    {
        --m_axis;
        return *this;
    }

    constexpr auto operator--(int) -> vector_iterator { return --auto(*this); }

    constexpr auto operator-=(difference_type n) -> vector_iterator&
    {
        m_axis -= n;
        return *this;
    }

    friend constexpr auto operator-(vector_iterator const& v, difference_type n) -> vector_iterator
    {
        return auto(v) -= n;
    }
    friend constexpr auto operator-(vector_iterator const& lhs, vector_iterator rhs) -> difference_type
    {
        return lhs.m_axis - rhs.m_axis;
    }

    constexpr auto operator[](difference_type n) const -> value_type { return *(*this + n); }

    constexpr auto operator==(vector_iterator const& other) const -> bool
    {
        if (m_vector == nullptr && other.m_vector == nullptr)
            return true;
        if (m_vector == nullptr && other.m_axis == 3)
            return true;
        if (other.m_vector == nullptr && m_axis == 3)
            return true;
        return m_vector == other.m_vector && m_axis == other.m_axis;
    }
    constexpr auto operator<=>(vector_iterator const& other) const -> std::partial_ordering
    {
        if (m_vector == nullptr && other.m_vector == nullptr)
            return std::partial_ordering::equivalent;
        if (m_vector == nullptr && other.m_axis == 3)
            return std::partial_ordering::equivalent;
        if (other.m_vector == nullptr && m_axis == 3)
            return std::partial_ordering::equivalent;
        if (m_vector != other.m_vector)
            return std::partial_ordering::unordered;
        if (m_axis < other.m_axis)
            return std::partial_ordering::less;
        return std::partial_ordering::greater;
    }

  private:
    Vector const*   m_vector = nullptr;
    difference_type m_axis   = 0;

    constexpr static std::array<typename Vector::mapped_type (*)(Vector const&), 3> s_mapping{
        [](Vector const& v) { return v.q().value(); },
        [](Vector const& v) { return v.r().value(); },
        [](Vector const& v) { return v.s().value(); },
    };

    constexpr explicit vector_iterator(Vector const* vector, difference_type axis)
        : m_vector(vector)
        , m_axis(axis)
    {
    }

    friend Vector;
};
} // namespace hex::detail

#endif // HEX_DETAIL_VECTOR_ITERATOR_HPP
