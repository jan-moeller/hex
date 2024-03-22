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

#ifndef HEX_DETAIL_TRANSFORMATION_UTILS_HPP
#define HEX_DETAIL_TRANSFORMATION_UTILS_HPP

#include "hex/vector/coordinate.hpp"
#include "hex/vector/coordinate_axis.hpp"
#include "hex/vector/rotation.hpp"
#include "hex/vector/rotation_steps.hpp"
#include "hex/vector/scaling.hpp"
#include "hex/vector/shearing.hpp"
#include "hex/vector/vector.hpp"

#include <array>
#include <numbers>
#include <type_traits>

#include <cassert>

namespace hex::detail
{
template<typename T> // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
using transformation_matrix = std::array<T, 6>;

// Identity matrix in column-major order, with the last line omitted (it is always 0 0 1).
template<typename T>
inline constexpr transformation_matrix<T> identity_matrix{1, 0, 0, 1, 0, 0};

// The hexagonal sine function.
template<typename T>
constexpr auto hex_sin(T x) noexcept -> T
{
    return 2 * std::numbers::inv_sqrt3_v<T> * std::sin(x);
}
// The forward hexagonal cosine function.
template<typename T>
constexpr auto fwd_hex_cos(T x) noexcept -> T
{
    return -std::cos(x) - std::sin(x) * std::numbers::inv_sqrt3_v<T>;
}
// The backward hexagonal cosine function.
template<typename T>
constexpr auto bwd_hex_cos(T x) noexcept -> T
{
    return std::cos(x) - std::sin(x) * std::numbers::inv_sqrt3_v<T>;
}

template<typename T>
constexpr auto translation_to_matrix(vector<T> translation) -> transformation_matrix<T>
{
    return {1, 0, 0, 1, translation.q().value(), translation.r().value()};
}

template<typename T>
constexpr auto rotation_steps_to_matrix(rotation_steps steps) -> transformation_matrix<T>
{
    switch (steps.clockwise_steps())
    {
    case 1: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return {0, 1, -1, 1, 0, 0};
    case 2: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return {-1, 1, -1, 0, 0, 0};
    case 3: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return {-1, 0, 0, -1, 0, 0};
    case 4: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return {0, -1, 1, -1, 0, 0};
    case 5: // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return {1, -1, 1, 0, 0, 0};
    default:
        return identity_matrix<T>;
    }
}

template<typename T>
constexpr auto rotation_to_matrix(rotation<T> radians) -> transformation_matrix<T>
{
    auto const a = fwd_hex_cos(-radians.radians());
    auto const b = hex_sin(-radians.radians());
    auto const c = bwd_hex_cos(-radians.radians());
    return {-a, -b, b, c, 0, 0};
}

template<typename T>
constexpr auto reflection_axis_to_matrix(coordinate_axis axis) -> transformation_matrix<T>
{
    switch (axis)
    {
    case coordinate_axis::q:
        return {-1, 1, 0, 1, 0, 0};
    case coordinate_axis::r:
        return {1, 0, 1, -1, 0, 0};
    case coordinate_axis::s:
        return {0, -1, -1, 0, 0, 0};
    }
    assert(false);
    return identity_matrix<T>;
}
template<typename T>
constexpr auto scaling_to_matrix(scaling<T> scale) -> transformation_matrix<T>
{
    if (auto const* p = std::get_if<T>(&scale.factor()))
        return {*p, 0, 0, *p, 0, 0};
    if (auto const* p = std::get_if<q_coordinate<T>>(&scale.factor()))
        return {1, (p->value() - 1) / 2, 0, p->value(), 0, 0};
    if (auto const* p = std::get_if<r_coordinate<T>>(&scale.factor()))
        return {p->value(), 0, (p->value() - 1) / 2, 1, 0, 0};
    if (auto const* p = std::get_if<s_coordinate<T>>(&scale.factor()))
        return {(1 + p->value()) / 2, (1 - p->value()) / 2, (1 - p->value()) / 2, (1 + p->value()) / 2, 0, 0};
    assert(false);
    return identity_matrix<T>;
}
template<typename T>
constexpr auto shearing_to_matrix(shearing<T> strain) -> transformation_matrix<T>
{
    if (auto const* p = std::get_if<q_coordinate<T>>(&strain.strain()))
        return {1, p->value(), 0, 1, 0, 0};
    if (auto const* p = std::get_if<r_coordinate<T>>(&strain.strain()))
        return {1, 0, -p->value(), 1, 0, 0};
    if (auto const* p = std::get_if<s_coordinate<T>>(&strain.strain()))
        return {1 - p->value(), p->value(), -p->value(), 1 + p->value(), 0, 0};
    assert(false);
    return identity_matrix<T>;
}

template<typename T, typename U>
constexpr auto matrix_cast(transformation_matrix<U> const& m) -> transformation_matrix<T>
{
    return {static_cast<T>(m[0]),  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            static_cast<T>(m[1]),  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            static_cast<T>(m[2]),  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            static_cast<T>(m[3]),  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            static_cast<T>(m[4]),  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            static_cast<T>(m[5])}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

template<typename T, typename U>
constexpr auto combine_matrix(transformation_matrix<T> const& m1,
                              transformation_matrix<U> const& m2) -> transformation_matrix<std::common_type_t<T, U>>
{
    // This is an inline matrix-matrix multiplication assuming the last line is 0 0 1.
    // Transforms apply right-to-left, i.e. the resulting transformation is as if m2 is applied before m1.
    auto const a = m1[0] * m2[0] + m1[2] * m2[1];
    auto const b = m1[1] * m2[0] + m1[3] * m2[1];
    auto const c = m1[0] * m2[2] + m1[2] * m2[3];
    auto const d = m1[1] * m2[2] + m1[3] * m2[3];
    auto const e = m1[0] * m2[4] + m1[2] * m2[5] + m1[4];
    auto const f = m1[1] * m2[4] + m1[3] * m2[5] + m1[5];
    return {a, b, c, d, e, f};
}

template<typename T>
struct matrix_wrapper
{
    transformation_matrix<T> mat;
};

template<typename T, typename U>
constexpr auto operator%(matrix_wrapper<T> const& lhs,
                         matrix_wrapper<U> const& rhs) -> matrix_wrapper<std::common_type_t<T, U>>
{
    // lhs & rhs intentionally reversed. Mathematically, the rightmost matrix is applied first, but we are used to think
    // left to right.
    return {combine_matrix(rhs.mat, lhs.mat)};
}

template<typename... Ts>
constexpr auto
combine_matrix_rtl(transformation_matrix<Ts> const&... ms) -> transformation_matrix<std::common_type_t<Ts...>>
{
    return (... % matrix_wrapper<Ts>{ms}).mat;
}

template<typename T, typename U>
constexpr auto apply_matrix(transformation_matrix<T> const& m, vector<U> const& vec) -> vector<std::common_type_t<T, U>>
{
    using R      = std::common_type_t<T, U>;
    auto const q = m[0] * vec.q().value() + m[2] * vec.r().value() + m[4];
    auto const r = m[1] * vec.q().value() + m[3] * vec.r().value() + m[5];
    return {q_coordinate<R>(q), r_coordinate<R>(r)};
}
} // namespace hex::detail

#endif // HEX_DETAIL_TRANSFORMATION_UTILS_HPP
