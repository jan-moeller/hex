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

#ifndef HEX_TRANSFORMATION_HPP
#define HEX_TRANSFORMATION_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/detail/detail_transformation_utils.hpp"
#include "hex/vector/reflection.hpp"
#include "hex/vector/rotation.hpp"
#include "hex/vector/rotation_steps.hpp"
#include "hex/vector/scaling.hpp"
#include "hex/vector/shearing.hpp"
#include "hex/vector/translation.hpp"
#include "hex/vector/vector.hpp"

#include <concepts>
#include <type_traits>

namespace hex
{
// Represents an affine transformation in hex space, such as translation, rotation, reflection, or a mixture thereof.
template<detail::arithmetic T = int>
class transformation
{
  public:
    // Creates the identity transformation.
    constexpr transformation();

    // Creates a transformation that translates by the given offset.
    constexpr transformation(translation<T> translation);

    // Creates a transformation that rotations in 60° steps.
    constexpr transformation(rotation_steps rotation);

    // Creates a transformation that rotates by any angle.
    constexpr transformation(rotation<T> radians)
        requires std::floating_point<T>;

    // Creates a transformation that reflects along an axis.
    constexpr transformation(reflection axis);

    // Creates a transformation that scales uniformly or along an axis.
    constexpr transformation(scaling<T> scale);

    // Creates a transformation that shears along an axis.
    constexpr transformation(shearing<T> strain);

    // Creates a transformation by combining several others. The resulting transformation's result is the same as
    // applying all passed transformations in left-to-right order.
    template<hex::detail::arithmetic Head, hex::detail::arithmetic... Ts>
        requires(!std::same_as<Head, T> || sizeof...(Ts) != 0)
    constexpr explicit transformation(transformation<Head> const& t, hex::transformation<Ts> const&... ts);

  private:
    detail::transformation_matrix<T> m_matrix;

    template<detail::arithmetic U, detail::arithmetic V>
    friend constexpr auto transform(vector<U> const& v, transformation<V> const& t) -> vector<std::common_type_t<U, V>>;

    template<detail::arithmetic U>
    friend class transformation;
};
template<typename... Ts>
transformation(transformation<Ts> const&...) -> transformation<std::common_type_t<Ts...>>;

// Combines several transformations into on. The resulting transformation's result is the same as applying all passed
// transformations in left-to-right order.
template<detail::arithmetic T, detail::arithmetic... Ts>
constexpr auto combine(transformation<T> const& t,
                       transformation<Ts> const&... ts) -> transformation<std::common_type_t<T, Ts...>>;

// Applies a transformation to a vector, returning the transformed vector.
template<detail::arithmetic T, detail::arithmetic U>
constexpr auto transform(vector<T> const& v, transformation<U> const& t) -> vector<std::common_type_t<T, U>>;
} // namespace hex

// ------------------------------ implementation below ------------------------------

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation()
    : m_matrix(detail::identity_matrix<T>)
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::translation<T> translation)
    : m_matrix(detail::translation_to_matrix(translation.offset()))
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::rotation_steps rotation)
    : m_matrix(detail::rotation_steps_to_matrix<T>(rotation))
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::rotation<T> radians)
    requires std::floating_point<T>
    : m_matrix(detail::rotation_to_matrix(radians))
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::reflection axis)
    : m_matrix(detail::reflection_axis_to_matrix<T>(axis.axis()))
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::scaling<T> scale)
    : m_matrix(detail::scaling_to_matrix(scale))
{
}

template<hex::detail::arithmetic T>
constexpr hex::transformation<T>::transformation(hex::shearing<T> strain)
    : m_matrix(detail::shearing_to_matrix(strain))
{
}

template<hex::detail::arithmetic T>
template<hex::detail::arithmetic Head, hex::detail::arithmetic... Ts>
    requires(!std::same_as<Head, T> || sizeof...(Ts) != 0)
constexpr hex::transformation<T>::transformation(transformation<Head> const& t, transformation<Ts> const&... ts)
    : m_matrix(detail::matrix_cast<T>(detail::combine_matrix_rtl(t.m_matrix, ts.m_matrix...)))
{
}

template<hex::detail::arithmetic T, hex::detail::arithmetic... Ts>
constexpr auto hex::combine(transformation<T> const& t,
                            transformation<Ts> const&... ts) -> transformation<std::common_type_t<T, Ts...>>
{
    return transformation<std::common_type_t<T, Ts...>>{t, ts...};
}

template<hex::detail::arithmetic T, hex::detail::arithmetic U>
constexpr auto hex::transform(vector<T> const& v, transformation<U> const& t) -> vector<std::common_type_t<T, U>>
{
    return detail::apply_matrix(t.m_matrix, v);
}
#endif // HEX_TRANSFORMATION_HPP
