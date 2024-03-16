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

#ifndef HEX_DETAIL_NEIGHBORS_HPP
#define HEX_DETAIL_NEIGHBORS_HPP

#include "hex/vector/vector.hpp"

#include <array>

#include <cstdint>

namespace hex::detail
{
inline constexpr std::array<vector<std::int8_t>, 6> neighbors{unit_qs, unit_qr, unit_sr, unit_sq, unit_rq, unit_rs};

inline constexpr std::array<vector<std::int8_t>, 6> diagonals{diag_q,
                                                              coordinate_cast<std::int8_t>(-diag_r),
                                                              diag_s,
                                                              coordinate_cast<std::int8_t>(-diag_q),
                                                              diag_r,
                                                              coordinate_cast<std::int8_t>(-diag_s)};

template<typename T>
struct apply_neighbor
{
    vector<T> center;

    constexpr auto operator()(vector<std::int8_t> v) const -> vector<T> const { return center + v; }
};
} // namespace hex::detail

#endif // HEX_DETAIL_NEIGHBORS_HPP
