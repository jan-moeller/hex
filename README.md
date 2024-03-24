# hex

[![linux](https://github.com/jan-moeller/hex/actions/workflows/linux.yml/badge.svg)](https://github.com/jan-moeller/hex/actions/workflows/linux.yml)

A header-only C++23 hexagonal grid library.

## Examples

### Storing Data on a Hex Grid

```c++
#include <hex/hex.hpp>
#include <print>

int main()
{
    using namespace hex;
    using namespace hex::literals;
    
    // Typedef for a grid that can have any convex polygon shape.
    using my_grid = grid<int, convex_polygon_view<int>>;

    // Create a grid with the shape of a regular polygon with radius=1.
    my_grid grid(views::convex_polygon(make_regular_hexagon_parameters(1)));

    // Set the data of tile (0, 0, 0) to 42.
    grid[{0_q, 0_r}] = 42;

    // Print all 7 tiles.
    for (auto [v, i] : grid)
        std::println("{}: {}", v, i);
}
```

Output:
```text
(-1, 0, 1): 0
(-1, 1, 0): 0
(0, -1, 1): 0
(0, 0, 0): 42
(0, 1, -1): 0
(1, -1, 0): 0
(1, 0, -1): 0
```

### Views

```c++
#include <hex/hex.hpp>
#include <print>

int main()
{
    using namespace hex;
    using namespace hex::literals;

    // Print all tiles on a line from (1, 2, -3) to (-1, -1, 2), but rotated by 60° around the origin.
    for (auto&& v : views::line(vector{1_q, 2_r}, vector{-1_q, 2_s}) | views::transform(rot_60_cw))
        std::println("{}", v);
}
```

Output:
```text
(-2, 3, -1)
(-1, 2, -1)
(-1, 1, 0)
(0, 0, 0)
(0, -1, 1)
(1, -2, 1)
```

### Conversion to and from Cartesian Coordinates

```c++
#include <hex/hex.hpp>
#include <print>

int main()
{
    using namespace hex;
    
    auto const vec = from_cartesian(std::array{1, 2});
    std::println("Fractional: {}", vec);
    std::println("Nearest: {}", round(vec));

    auto const pos = to_cartesian(round(vec));
    std::println("Cartesian of Nearest: ({}, {})", pos[0], pos[1]);
}
```

Output:
```text
Fractional: (0.6666666666666666, 0.8213672050459181, -1.4880338717125847)
Nearest: (1, 1, -2)
Cartesian of Nearest: (1.5, 2.598076211353316)
```