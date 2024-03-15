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

#ifndef HEX_DETAIL_GENERATING_RANDOM_ACCESS_ITERATOR_HPP
#define HEX_DETAIL_GENERATING_RANDOM_ACCESS_ITERATOR_HPP

#include <algorithm>
#include <compare>
#include <type_traits>

#include <cstddef>

namespace hex::detail
{
template<typename Fn>
class generating_random_access_iterator
{
  public:
    using value_type      = std::invoke_result_t<Fn, std::size_t> const;
    using difference_type = std::ptrdiff_t;

    constexpr generating_random_access_iterator() = default;
    constexpr explicit generating_random_access_iterator(Fn fn, std::size_t idx = 0ZU)
        : m_index(idx)
        , m_generate(std::move(fn))
    {
    }

    constexpr auto operator++() noexcept -> generating_random_access_iterator&
    {
        ++m_index;
        return *this;
    }
    constexpr auto operator++(int) noexcept -> generating_random_access_iterator
    {
        auto copy = *this;
        ++m_index;
        return copy;
    }
    friend constexpr auto operator+(generating_random_access_iterator const& iter,
                                    difference_type n) noexcept -> generating_random_access_iterator
    {
        return generating_random_access_iterator{iter.m_generate, iter.m_index + n};
    }
    friend constexpr auto operator+(difference_type n, generating_random_access_iterator const& iter) noexcept
        -> generating_random_access_iterator
    {
        return iter + n;
    }
    constexpr auto operator+=(difference_type n) noexcept -> generating_random_access_iterator&
    {
        m_index += n;
        return *this;
    }

    constexpr auto operator--() noexcept -> generating_random_access_iterator&
    {
        --m_index;
        return *this;
    }
    constexpr auto operator--(int) noexcept -> generating_random_access_iterator
    {
        auto copy = *this;
        --m_index;
        return copy;
    }
    friend constexpr auto operator-(generating_random_access_iterator const& iter,
                                    difference_type n) noexcept -> generating_random_access_iterator
    {
        return generating_random_access_iterator{iter.m_generate, iter.m_index - n};
    }
    friend constexpr auto operator-(generating_random_access_iterator const& lhs,
                                    generating_random_access_iterator const& rhs) noexcept -> difference_type
    {
        return lhs.m_index - rhs.m_index;
    }
    constexpr auto operator-=(difference_type n) noexcept -> generating_random_access_iterator&
    {
        m_index -= n;
        return *this;
    }

    // NOLINTNEXTLINE(readability-const-return-type)
    constexpr auto operator*() const -> value_type { return m_generate(m_index); }

    // NOLINTNEXTLINE(readability-const-return-type)
    constexpr auto operator[](difference_type n) const -> value_type { return m_generate(m_index + n); }

    friend constexpr auto operator==(generating_random_access_iterator const& lhs,
                                     generating_random_access_iterator const& rhs) -> bool
    {
        return lhs.m_index == rhs.m_index;
    }
    friend constexpr auto operator<=>(generating_random_access_iterator const& lhs,
                                      generating_random_access_iterator const& rhs) -> std::strong_ordering
    {
        if (lhs == rhs)
            return std::strong_ordering::equal;
        return lhs.m_index < rhs.m_index ? std::strong_ordering::less : std::strong_ordering::greater;
    }

  private:
    std::size_t m_index{};
    Fn          m_generate;
};
} // namespace hex::detail

#endif // HEX_DETAIL_GENERATING_RANDOM_ACCESS_ITERATOR_HPP
