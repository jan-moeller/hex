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

#ifndef HEX_DETAIL_LINE_ITERATOR_HPP
#define HEX_DETAIL_LINE_ITERATOR_HPP

#include "hex/vector/coordinate.hpp"
#include "hex/vector/vector.hpp"

#include <bitset>

#include <cstddef>

namespace hex::detail
{
constexpr void m1_move(int& /*q*/, // NOLINT(bugprone-easily-swappable-parameters)
                       int& r,     // NOLINT(bugprone-easily-swappable-parameters)
                       int& delta, // NOLINT(bugprone-easily-swappable-parameters)
                       int  dq,    // NOLINT(bugprone-easily-swappable-parameters)
                       int /*dr*/, // NOLINT(bugprone-easily-swappable-parameters)
                       int dir)    // NOLINT(bugprone-easily-swappable-parameters)
{
    r += dir;
    delta -= 2 * dq * dir;
};
constexpr void m2_move(int& q,     // NOLINT(bugprone-easily-swappable-parameters)
                       int& /*r*/, // NOLINT(bugprone-easily-swappable-parameters)
                       int& delta, // NOLINT(bugprone-easily-swappable-parameters)
                       int /*dq*/, // NOLINT(bugprone-easily-swappable-parameters)
                       int dr,     // NOLINT(bugprone-easily-swappable-parameters)
                       int dir)    // NOLINT(bugprone-easily-swappable-parameters)
{
    q += dir;
    delta += 2 * dr * dir;
};
constexpr void m3_move(int& q,     // NOLINT(bugprone-easily-swappable-parameters)
                       int& r,     // NOLINT(bugprone-easily-swappable-parameters)
                       int& delta, // NOLINT(bugprone-easily-swappable-parameters)
                       int  dq,    // NOLINT(bugprone-easily-swappable-parameters)
                       int  dr,    // NOLINT(bugprone-easily-swappable-parameters)
                       int  dir)    // NOLINT(bugprone-easily-swappable-parameters)
{
    q += dir;
    r -= dir;
    delta += 2 * (dq + dr) * dir;
}

template<typename T>
class line_iterator
{
  public:
    using difference_type = std::ptrdiff_t;
    using value_type      = vector<T> const;

    constexpr line_iterator() noexcept = default;
    constexpr line_iterator(vector<T> const& p1, vector<T> const& p2) noexcept
        : m_dq(p2.q().value() - p1.q().value())
        , m_dr(p2.r().value() - p1.r().value())
        , m_q(p1.q().value())
        , m_r(p1.r().value())
    {
        if (m_dq >= 0)
        {
            if (m_dr > 0)
            {
                m_delta                    = m_dr - m_dq;
                m_flags[s_update1_dir_bit] = false;
                m_flags[s_update2_dir_bit] = false;
                m_update1                  = &m1_move;
                m_update2                  = &m2_move;
                m_remaining                = m_dq + m_dr;
            }
            else if (m_dq > -m_dr)
            {
                m_delta                    = 2 * m_dr + m_dq;
                m_flags[s_update1_dir_bit] = false;
                m_flags[s_update2_dir_bit] = false;
                m_update1                  = &m2_move;
                m_update2                  = &m3_move;
                m_remaining                = m_dq;
            }
            else
            {
                m_delta                    = m_dr + 2 * m_dq;
                m_flags[s_update1_dir_bit] = false;
                m_flags[s_update2_dir_bit] = true;
                m_update1                  = &m3_move;
                m_update2                  = &m1_move;
                m_remaining                = -m_dr;
            }
        }
        else
        {
            if (m_dr < 0)
            {
                m_delta                    = m_dq - m_dr;
                m_flags[s_update1_dir_bit] = true;
                m_flags[s_update2_dir_bit] = true;
                m_update1                  = &m1_move;
                m_update2                  = &m2_move;
                m_remaining                = -m_dq - m_dr;
            }
            else if (-m_dq > m_dr)
            {
                m_delta                    = -2 * m_dr - m_dq;
                m_flags[s_update1_dir_bit] = true;
                m_flags[s_update2_dir_bit] = true;
                m_update1                  = &m2_move;
                m_update2                  = &m3_move;
                m_remaining                = -m_dq;
            }
            else
            {
                m_delta                    = -2 * m_dq - m_dr;
                m_flags[s_update1_dir_bit] = true;
                m_flags[s_update2_dir_bit] = false;
                m_update1                  = &m3_move;
                m_update2                  = &m1_move;
                m_remaining                = -m_dr;
            }
        }
    }

    constexpr auto operator++() noexcept -> line_iterator&
    {
        --m_remaining;
        if (m_delta > 0)
            m_update1(m_q, m_r, m_delta, m_dq, m_dr, m_flags[s_update1_dir_bit] ? -1 : 1);
        else
            m_update2(m_q, m_r, m_delta, m_dq, m_dr, m_flags[s_update2_dir_bit] ? -1 : 1);
        return *this;
    }
    constexpr auto operator++(int) noexcept -> line_iterator
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    [[nodiscard]] constexpr auto operator*() const noexcept -> vector<T> const // NOLINT(readability-const-return-type)
    {
        return {q_coordinate<T>{m_q}, r_coordinate<T>{m_r}};
    }

    constexpr auto operator==(line_iterator const& other) const -> bool { return m_remaining == other.m_remaining; };

  private:
    std::size_t m_remaining = 0;
    T           m_dq;
    T           m_dr;
    T           m_q;
    T           m_r;
    T           m_delta;
    void (*m_update1)(T&, T&, T&, T, T, int);
    void (*m_update2)(T&, T&, T&, T, T, int);
    std::bitset<2> m_flags;

    static constexpr int s_update1_dir_bit = 0;
    static constexpr int s_update2_dir_bit = 1;
};
} // namespace hex::detail

#endif // HEX_DETAIL_LINE_ITERATOR_HPP
