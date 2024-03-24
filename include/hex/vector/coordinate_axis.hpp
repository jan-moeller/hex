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

#ifndef HEX_COORDINATE_AXIS_HPP
#define HEX_COORDINATE_AXIS_HPP

#include <algorithm>
#include <array>
#include <format>
#include <utility>

#include <cstdint>

namespace hex
{
// Designates the 3 hex grid coordinate axes, q, r and s.
enum class coordinate_axis : std::uint8_t
{
    q, // The q coordinate axis.
    r, // The r coordinate axis.
    s, // The s coordinate axis.
};
} // namespace hex

template<typename CharT>
struct std::formatter<hex::coordinate_axis, CharT>
{
    bool uppercase = false;

    template<class ParseContext>
    constexpr auto parse(ParseContext& ctx) -> ParseContext::iterator;

    template<class FmtContext>
    constexpr auto format(hex::coordinate_axis axis, FmtContext& ctx) const -> FmtContext::iterator;
};

// ------------------------------ implementation below ------------------------------

template<typename CharT>
template<class ParseContext>
constexpr auto std::formatter<hex::coordinate_axis, CharT>::parse(ParseContext& ctx) -> ParseContext::iterator
{
    auto it = ctx.begin();
    if (it == ctx.end() || *it == CharT{'}'})
        return it;
    if (*it == CharT{'U'})
    {
        uppercase = true;
        ++it;
    }
    if (*it != CharT{'}'})
        throw std::format_error("Invalid format string.");
    return it;
}

template<typename CharT>
template<class FmtContext>
constexpr auto std::formatter<hex::coordinate_axis, CharT>::format(hex::coordinate_axis axis,
                                                                   FmtContext& ctx) const -> FmtContext::iterator
{
    static constexpr std::array<std::array<char const*, 4>, 2> chars = {
        {{"q", "r", "s", "<invalid>"}, {"Q", "R", "S", "<INVALID>"}}};

    auto&&    out = ctx.out();
    int const i   = uppercase;
    int const j   = std::min(std::to_underlying(axis), std::uint8_t{3});
    return std::format_to(out, "{}", chars[i][j]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
}

#endif // HEX_COORDINATE_AXIS_HPP
