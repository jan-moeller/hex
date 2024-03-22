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

#ifndef HEX_TRANSLATION_HPP
#define HEX_TRANSLATION_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/vector.hpp"

namespace hex
{
// A helper type holding information about a translation transformation.
template<detail::arithmetic T>
class translation
{
  public:
    // Creates a translation by the given offset.
    constexpr explicit translation(vector<T> offset) noexcept
        : m_offset(offset)
    {
    }

    // Retrieves the offset.
    [[nodiscard]] constexpr auto offset() const noexcept -> vector<T> const& { return m_offset; }

  private:
    vector<T> m_offset;
};
template<detail::arithmetic T>
translation(vector<T>) -> translation<T>;
} // namespace hex

#endif // HEX_TRANSLATION_HPP
