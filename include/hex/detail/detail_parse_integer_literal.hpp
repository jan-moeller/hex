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

#ifndef HEX_DETAIL_PARSE_INTEGER_LITERAL_HPP
#define HEX_DETAIL_PARSE_INTEGER_LITERAL_HPP

#include <charconv>
#include <optional>
#include <string>
#include <system_error>
#include <vector>

namespace hex::detail
{
template<char... Digits>
consteval auto parse_integer_literal() // Parses into int, long, or long long
{
    constexpr auto parse = []<typename T>() -> std::optional<T>
    {
        T           result{};
        std::string raw_str{Digits...};
        erase(raw_str, '\'');
        char const* begin = raw_str.data();
        char const* end   = raw_str.data() + raw_str.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

        auto [ptr, ec] = std::from_chars(begin, end, result);
        if (ec == std::errc() && ptr == end)
            return result;
        if (ec == std::errc::invalid_argument)
            throw "not a valid number"; // NOLINT(hicpp-exception-baseclass) Only for compile-time error reporting
        return std::nullopt;
    };

    if constexpr (parse.template operator()<int>())
        return *parse.template operator()<int>();
    else if constexpr (parse.template operator()<long>())
        return *parse.template operator()<long>();
    else if constexpr (parse.template operator()<long long>())
        return *parse.template operator()<long long>();

    throw "doesn't fit into any number type"; // NOLINT(hicpp-exception-baseclass) Only for compile-time error reporting
}
} // namespace hex::detail

#endif // HEX_DETAIL_PARSE_INTEGER_LITERAL_HPP
