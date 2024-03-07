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

#ifndef HEX_GRID_HPP
#define HEX_GRID_HPP

#include "hex/detail/detail_grid_iterator.hpp"

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

#include <cstddef>

namespace hex
{
// Matches types usable as a shape for hex::grid
template<class T>
concept grid_shape = std::ranges::sized_range<T> && std::ranges::common_range<T> && std::ranges::bidirectional_range<T>
                     && requires(T t, std::ranges::range_value_t<T> v) { // clang-format off: requires on single line
                            { t.index_of(v) } -> std::convertible_to<std::size_t>; // clang-format on
                        };

// A fixed-size associative container mapping hex positions to user-defined data. All storage is contiguous, and
// allocations are only made on construction and copy.
// This type models std::ranges::sized_range, std::ranges::bidirectional_range, std::ranges::common_range.
template<typename T, grid_shape Shape, class Allocator = std::allocator<T>>
class grid
{
  public:
    using allocator_type         = Allocator;
    using shape_type             = Shape;
    using key_type               = std::ranges::range_value_t<Shape>;
    using mapped_type            = T;
    using value_type             = std::pair<key_type const, T>;
    using reference              = std::pair<std::ranges::range_const_reference_t<Shape>, mapped_type&>;
    using const_reference        = std::pair<std::ranges::range_const_reference_t<Shape>, mapped_type const&>;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using iterator               = detail::grid_iterator<grid, false>;
    using const_iterator         = detail::grid_iterator<grid, true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr ~grid() = default;

    // Initializes the grid using a default-constructed shape filled with default-constructed Ts.
    constexpr grid();

    constexpr grid(grid const&) = default;
    constexpr grid(grid const& other, Allocator const& alloc);
    constexpr grid(grid&&) = default;
    constexpr grid(grid&& other, Allocator const& alloc);

    // Initializes the grid with the given shape (and potentially allocator).
    constexpr explicit grid(Shape const& shape, Allocator const& alloc = Allocator());

    // Initializes the grid with a default-constructed shape and the given allocator.
    constexpr explicit grid(Allocator const& alloc);

    // Initializes the grid with the given shape and allocator, setting all values from the given range; keys contained
    // in the grid not mentioned in the range are default-constructed. The range must not contain any keys outside the
    // given shape.
    template<std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    constexpr grid(InputIt first, Sentinel last, Shape const& shape = Shape(), Allocator const& alloc = Allocator());

    // Initializes the grid with the given allocator and default-constructed shape, setting all values from the given
    // range; keys contained in the grid not mentioned in the range are default-constructed. The range must not contain
    // any keys outside the given shape.
    template<std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    constexpr grid(InputIt first, Sentinel last, Allocator const& alloc);

    // Initializes the grid with the given allocator and shape, setting all values from the given initializer_list; keys
    // contained in the grid not mentioned in the list are default-constructed. The list must not contain any keys
    // outside the given shape.
    constexpr grid(std::initializer_list<value_type> init,
                   Shape const&                      shape = Shape(),
                   Allocator const&                  alloc = Allocator());

    // Initializes the grid with the given allocator and default-constructed shape, setting all values from the given
    // initializer_list; keys contained in the grid not mentioned in the list are default-constructed. The list must not
    // contain any keys outside the given shape.
    constexpr grid(std::initializer_list<value_type> init, Allocator const& alloc);

#if __cpp_lib_containers_ranges >= 202202L
    template<detail::container_compatible_range<value_type> R>
    constexpr grid(std::from_range_t, R&& rg, Policy const& shape = Policy(), Allocator const& alloc = Allocator());
    template<detail::container_compatible_range<value_type> R>
    constexpr grid(std::from_range_t, R&& rg, Allocator const& alloc);
#endif

    constexpr auto operator=(grid const&) -> grid& = default;
    constexpr auto operator=(grid&&) -> grid&      = default;

    // Returns reference to value associated with the given key. UB if key outside of shape.
    constexpr auto operator[](key_type const& key) -> T&;
    // Returns reference to value associated with the given key. UB if key outside of shape.
    constexpr auto operator[](key_type&& key) -> T&;
    // Returns const reference to value associated with the given key. UB if key outside of shape.
    constexpr auto operator[](key_type const& key) const -> T const&;
    // Returns const reference to value associated with the given key. UB if key outside of shape.
    constexpr auto operator[](key_type&& key) const -> T const&;

    // Returns reference to value associated with the given key. Throws std::out_of_range if key outside of shape. Note
    // that this calls contains() to determine whether the given key is valid; depending on the implementation of Shape,
    // this might be O(n)!
    constexpr auto at(key_type const& key) -> T&;
    // Returns const reference to value associated with the given key. Throws std::out_of_range if key outside of shape.
    // Note that this calls contains() to determine whether the given key is valid; depending on the implementation of
    // Shape, this might be O(n)!
    constexpr auto at(key_type const& key) const -> T const&;

    constexpr auto begin() noexcept -> iterator;
    constexpr auto begin() const noexcept -> const_iterator;
    constexpr auto cbegin() const noexcept -> const_iterator;

    constexpr auto end() noexcept -> iterator;
    constexpr auto end() const noexcept -> const_iterator;
    constexpr auto cend() const noexcept -> const_iterator;

    constexpr auto rbegin() noexcept -> reverse_iterator;
    constexpr auto rbegin() const noexcept -> const_reverse_iterator;
    constexpr auto crbegin() const noexcept -> const_reverse_iterator;

    constexpr auto rend() noexcept -> reverse_iterator;
    constexpr auto rend() const noexcept -> const_reverse_iterator;
    constexpr auto crend() const noexcept -> const_reverse_iterator;

    // Returns true if shape has no elements, otherwise false.
    [[nodiscard]] constexpr auto empty() const noexcept -> bool;
    // Returns number of keys in the grid.
    [[nodiscard]] constexpr auto size() const noexcept -> size_type;
    // Returns size().
    [[nodiscard]] constexpr auto max_size() const noexcept -> size_type;

    constexpr void swap(grid& other) noexcept;

    // Returns an iterator to the given key, if found. Otherwise, returns end(). If Shape implements a find() function,
    // this implementation is used. Otherwise, std::ranges::find is used. Note that, in the latter case, this operation
    // is O(n).
    constexpr auto find(key_type const& key) -> iterator;
    // Returns an iterator to the given key, if found. Otherwise, returns end(). If Shape implements a find() function,
    // this implementation is used. Otherwise, std::ranges::find is used. Note that, in the latter case, this operation
    // is O(n).
    constexpr auto find(key_type const& key) const -> const_iterator;

    // Returns 1 if the key is in the grid, otherwise 0. Has the same performance characteristics as contains().
    constexpr auto count(key_type const& key) const -> size_type;

    // Returns true if the given key is in the grid, otherwise false. If Shape implements a contains() function, this
    // implementation is used. Otherwise, if it implements a find() function, it is used. Otherwise,
    // std::ranges::contains is used. Note that, in the latter case, this operation is O(n).
    constexpr auto contains(key_type const& key) const -> bool;

    friend constexpr auto operator==(grid const& lhs, grid const& rhs) -> bool = default;
    friend constexpr auto operator<=>(grid const& lhs, grid const& rhs)        = default;

    template<typename U, class P, class A>
    friend constexpr void swap(grid<U, P, A>& lhs, grid<U, P, A>& rhs) noexcept;

  private:
    std::vector<mapped_type, Allocator> m_data;
    Shape                               m_shape;
};

// ------------------------------ implementation below ------------------------------

template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid()
    : grid(Shape())
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(grid const& other, Allocator const& alloc)
    : m_data(other.m_data, alloc)
    , m_shape(other.m_shape)
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(grid&& other, Allocator const& alloc)
    : m_data(std::move(other).m_data, alloc)
    , m_shape(std::move(other).m_shape)
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(Shape const& shape, Allocator const& alloc)
    : m_data(shape.size(), alloc)
    , m_shape(shape)
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(Allocator const& alloc)
    : grid(Shape(), alloc)
{
}
template<typename T, grid_shape Shape, class Allocator>
template<std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
constexpr grid<T, Shape, Allocator>::grid(InputIt first, Sentinel last, Shape const& shape, Allocator const& alloc)
    : grid(shape, alloc)
{
    for (auto&& iter = first; iter != last; ++iter)
        (*this)[iter->first] = iter->second;
}
template<typename T, grid_shape Shape, class Allocator>
template<std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
constexpr grid<T, Shape, Allocator>::grid(InputIt first, Sentinel last, Allocator const& alloc)
    : grid(first, last, Shape(), alloc)
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(std::initializer_list<value_type> init,
                                          Shape const&                      shape,
                                          Allocator const&                  alloc)
    : grid(init.begin(), init.end(), shape, alloc)
{
}
template<typename T, grid_shape Shape, class Allocator>
constexpr grid<T, Shape, Allocator>::grid(std::initializer_list<value_type> init, Allocator const& alloc)
    : grid(init, Shape(), alloc)
{
}
#if __cpp_lib_containers_ranges >= 202202L
template<detail::container_compatible_range<value_type> R>
constexpr grid<T, Shape, Allocator>::grid(std::from_range_t, R&& rg, Shape const& shape, Allocator const& alloc)
    : grid(std::forward<R>(rg).begin(), std::forward<R>(rg).end(), shape, alloc)
{
}
template<detail::container_compatible_range<value_type> R>
constexpr grid<T, Shape, Allocator>::grid(std::from_range_t, R&& rg, Allocator const& alloc)
    : grid(std::forward<R>(rg).begin(), std::forward<R>(rg).end(), Shape(), alloc)
{
}
#endif
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::operator[](key_type const& key) -> T&
{
    return m_data[m_shape.index_of(key)];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::operator[](key_type&& key) -> T&
{
    return m_data[m_shape.index_of(std::move(key))];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::operator[](key_type const& key) const -> T const&
{
    return m_data[m_shape.index_of(key)];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::operator[](key_type&& key) const -> T const&
{
    return m_data[m_shape.index_of(std::move(key))];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::at(key_type const& key) -> T&
{
    if (!contains(key))
        throw std::out_of_range("grid::at");
    return (*this)[key];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::at(key_type const& key) const -> T const&
{
    if (!contains(key))
        throw std::out_of_range("grid::at");
    return (*this)[key];
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::begin() noexcept -> iterator
{
    return iterator(this, std::ranges::begin(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::begin() const noexcept -> const_iterator
{
    return const_iterator(this, std::ranges::cbegin(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::cbegin() const noexcept -> const_iterator
{
    return const_iterator(this, std::ranges::cbegin(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::end() noexcept -> iterator
{
    return iterator(this, std::ranges::end(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::end() const noexcept -> const_iterator
{
    return const_iterator(this, std::ranges::cend(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::cend() const noexcept -> const_iterator
{
    return const_iterator(this, std::ranges::cend(m_shape));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::rbegin() noexcept -> reverse_iterator
{
    return reverse_iterator(end());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::rbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::crbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::rend() noexcept -> reverse_iterator
{
    return reverse_iterator(begin());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::rend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::crend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::empty() const noexcept -> bool
{
    return m_data.empty();
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::size() const noexcept -> size_type
{
    return m_data.size();
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::max_size() const noexcept -> size_type
{
    return size();
}
template<typename T, grid_shape Shape, class Allocator>
constexpr void grid<T, Shape, Allocator>::swap(grid& other) noexcept
{
    m_data.swap(other.m_data);
    using std::swap;
    swap(m_shape, other.m_shape);
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::find(key_type const& key) -> iterator
{
    if constexpr (requires(Shape s) { s.find(key); })
        return iterator(this, m_shape.find(key));
    else
        return iterator(this, std::ranges::find(m_shape, key));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::find(key_type const& key) const -> const_iterator
{
    if constexpr (requires(Shape s) { s.find(key); })
        return const_iterator(this, m_shape.find(key));
    else
        return const_iterator(this, std::ranges::find(m_shape, key));
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::count(key_type const& key) const -> size_type
{
    return contains(key);
}
template<typename T, grid_shape Shape, class Allocator>
constexpr auto grid<T, Shape, Allocator>::contains(key_type const& key) const -> bool
{
    if constexpr (requires(Shape s) { s.contains(key); })
        return m_shape.contains(key);
    else if constexpr (requires(Shape s) { s.find(key); })
        return find(key) != end();
    else
        return std::ranges::contains(m_shape, key);
}
template<typename T, grid_shape Shape, class Allocator>
constexpr void swap(grid<T, Shape, Allocator>& lhs, grid<T, Shape, Allocator>& rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace hex

#endif // HEX_GRID_HPP
