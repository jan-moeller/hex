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

#ifndef HEX_TRANSFORM_VIEW_HPP
#define HEX_TRANSFORM_VIEW_HPP

#include "hex/detail/detail_arithmetic.hpp"
#include "hex/vector/reflection.hpp"
#include "hex/vector/rotation.hpp"
#include "hex/vector/rotation_steps.hpp"
#include "hex/vector/scaling.hpp"
#include "hex/vector/shearing.hpp"
#include "hex/vector/transformation.hpp"
#include "hex/vector/translation.hpp"
#include "hex/views/transform/detail/detail_transform_view.hpp"

#include <concepts>
#include <ranges>

namespace hex
{
// A specialization of std::ranges::transform_view that applies a hex transformation to a range of vectors.
template<std::ranges::input_range V, detail::arithmetic T>
using transform_view = std::ranges::transform_view<V, detail::apply_transformation<T>>;

namespace views
{
// Range adapter applying a transform to a range of vectors.
template<hex::detail::arithmetic T>
constexpr auto transform(transformation<T> transform);

// Range adapter applying a reflection transform to a range of vectors.
constexpr auto transform(reflection transform);

// Range adapter applying a rotation transform to a range of vectors.
template<std::floating_point T>
constexpr auto transform(rotation<T> transform);

// Range adapter applying a rotation transform to a range of vectors.
constexpr auto transform(rotation_steps transform);

// Range adapter applying a scaling transform to a range of vectors.
template<hex::detail::arithmetic T>
constexpr auto transform(scaling<T> transform);

// Range adapter applying a shearing transform to a range of vectors.
template<hex::detail::arithmetic T>
constexpr auto transform(shearing<T> transform);

// Range adapter applying a translation transform to a range of vectors.
template<hex::detail::arithmetic T>
constexpr auto transform(translation<T> transform);

// Applies a transform to a range of vectors.
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto transform(R&& range, transformation<T> transform);

// Applies a reflection transform to a range of vectors.
template<std::ranges::viewable_range R>
constexpr auto transform(R&& range, reflection transform);

// Applies a rotation transform to a range of vectors.
template<std::ranges::viewable_range R, std::floating_point T>
constexpr auto transform(R&& range, rotation<T> transform);

// Applies a rotation transform to a range of vectors.
template<std::ranges::viewable_range R>
constexpr auto transform(R&& range, rotation_steps transform);

// Applies a scaling transform to a range of vectors.
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto transform(R&& range, scaling<T> transform);

// Applies a shearing transform to a range of vectors.
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto transform(R&& range, shearing<T> transform);

// Applies a translation transform to a range of vectors.
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto transform(R&& range, translation<T> transform);
} // namespace views
} // namespace hex

// ------------------------------ implementation below ------------------------------

template<hex::detail::arithmetic T>
constexpr auto hex::views::transform(hex::transformation<T> transform)
{
    return std::views::transform(detail::apply_transformation{.transform = transform});
}

constexpr auto hex::views::transform(hex::reflection transform)
{
    return views::transform(transformation{transform});
}
template<std::floating_point T>
constexpr auto hex::views::transform(hex::rotation<T> transform)
{
    return views::transform(transformation{transform});
}
constexpr auto hex::views::transform(hex::rotation_steps transform)
{
    return views::transform(transformation{transform});
}
template<hex::detail::arithmetic T>
constexpr auto hex::views::transform(hex::scaling<T> transform)
{
    return views::transform(transformation{transform});
}
template<hex::detail::arithmetic T>
constexpr auto hex::views::transform(hex::shearing<T> transform)
{
    return views::transform(transformation{transform});
}
template<hex::detail::arithmetic T>
constexpr auto hex::views::transform(hex::translation<T> transform)
{
    return views::transform(transformation{transform});
}

template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto hex::views::transform(R&& range, hex::transformation<T> transform)
{
    return std::views::transform(std::forward<R>(range), detail::apply_transformation{.transform = transform});
}
template<std::ranges::viewable_range R>
constexpr auto hex::views::transform(R&& range, hex::reflection transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
template<std::ranges::viewable_range R, std::floating_point T>
constexpr auto hex::views::transform(R&& range, hex::rotation<T> transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
template<std::ranges::viewable_range R>
constexpr auto hex::views::transform(R&& range, hex::rotation_steps transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto hex::views::transform(R&& range, hex::scaling<T> transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto hex::views::transform(R&& range, hex::shearing<T> transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
template<std::ranges::viewable_range R, hex::detail::arithmetic T>
constexpr auto hex::views::transform(R&& range, hex::translation<T> transform)
{
    return views::transform(std::forward<R>(range), transformation{transform});
}
#endif // HEX_TRANSFORM_VIEW_HPP
