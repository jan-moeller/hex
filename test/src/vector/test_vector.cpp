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
#include "hex/vector/vector.hpp"

#include <catch2/catch_all.hpp>

#include <ranges>

using namespace hex;

TEST_CASE("vector")
{
    using namespace literals;

    SECTION("trivially copyable")
    {
        STATIC_CHECK(std::is_trivially_copyable_v<vector<int>>);
        STATIC_CHECK(std::is_trivially_copyable_v<vector<int>>);
        STATIC_CHECK(std::is_trivially_copyable_v<vector<int>>);
    }
    SECTION("default constructor")
    {
        constexpr vector v;
        STATIC_CHECK(v.q() == 0_q);
        STATIC_CHECK(v.r() == 0_r);
        STATIC_CHECK(v.s() == 0_s);
    }
    SECTION("construction from q + r")
    {
        constexpr vector v{1_q, 2_r};
        STATIC_CHECK(v.q() == 1_q);
        STATIC_CHECK(v.r() == 2_r);
        STATIC_CHECK(v.s() == -3_s);
    }
    SECTION("construction from q + s")
    {
        constexpr vector v{1_q, 2_s};
        STATIC_CHECK(v.q() == 1_q);
        STATIC_CHECK(v.r() == -3_r);
        STATIC_CHECK(v.s() == 2_s);
    }
    SECTION("construction from r + s")
    {
        constexpr vector v{1_r, 2_s};
        STATIC_CHECK(v.q() == -3_q);
        STATIC_CHECK(v.r() == 1_r);
        STATIC_CHECK(v.s() == 2_s);
    }
    SECTION("construction from q + r + s")
    {
        constexpr vector v{-3_q, 1_r, 2_s};
        STATIC_CHECK(v.q() == -3_q);
        STATIC_CHECK(v.r() == 1_r);
        STATIC_CHECK(v.s() == 2_s);

        CHECK_THROWS_AS((vector{1_q, 1_r, 1_s}), std::invalid_argument);
    }
    SECTION("construction from different underlying type")
    {
        vector<int> v1 = vector(coordinate_cast<signed char>(1_q), coordinate_cast<signed char>(1_r));
        CHECK(v1.q() == 1_q);
        CHECK(v1.r() == 1_r);

        vector<signed char> v2 = coordinate_cast<signed char>(vector<int>(1_q, 1_r));
        CHECK(v2.q() == 1_q);
        CHECK(v2.r() == 1_r);
    }
    SECTION("set")
    {
        vector v{1_q, 2_r};
        CHECK(v == vector{1_q, 2_r});
        v.set(2_q, 1_r);
        CHECK(v == vector{2_q, 1_r});
        v.set(1_q, 1_s);
        CHECK(v == vector{1_q, 1_s});
        v.set(1_r, 2_s);
        CHECK(v == vector{1_r, 2_s});
        v.set(1_q, 2_r, coordinate_cast<signed char>(-3_s));
        CHECK(v == vector{1_q, 2_r});
        CHECK_THROWS_AS(v.set(1_q, 1_r, 1_s), std::invalid_argument);
    }
    SECTION("unary +")
    {
        constexpr vector v{1_q, 2_r};
        STATIC_CHECK(+v == v);
    }
    SECTION("unary -")
    {
        constexpr vector v{1_q, 2_r};
        STATIC_CHECK(-v == vector{-1_q, -2_r});
    }
    SECTION("binary +")
    {
        constexpr vector lhs{1_q, 2_r};
        constexpr vector rhs{-1_q, 1_r};
        STATIC_CHECK(lhs + rhs == vector{0_q, 3_r});
    }
    SECTION("binary -")
    {
        constexpr vector lhs{1_q, 2_r};
        constexpr vector rhs{-1_q, 1_r};
        STATIC_CHECK(lhs - rhs == vector{2_q, 1_r});
    }
    SECTION("binary *")
    {
        constexpr vector lhs{1_q, 2_r};
        STATIC_CHECK(lhs * 2 == vector{2_q, 4_r});
        STATIC_CHECK(2 * lhs == vector{2_q, 4_r});
    }
    SECTION("binary /")
    {
        constexpr vector lhs{2_q, 4_r};
        STATIC_CHECK(lhs / 2 == vector{1_q, 2_r});
    }
    SECTION("binary +=")
    {
        vector<int> lhs{1_q, 2_r};
        vector<int> rhs{-1_q, 1_r};
        lhs += rhs;
        CHECK(lhs == vector{0_q, 3_r});
    }
    SECTION("binary -=")
    {
        vector<int> lhs{1_q, 2_r};
        vector<int> rhs{-1_q, 1_r};
        lhs -= rhs;
        CHECK(lhs == vector{2_q, 1_r});
    }
    SECTION("binary *=")
    {
        vector lhs{1_q, 2_r};
        lhs *= 2;
        CHECK(lhs == vector{2_q, 4_r});
    }
    SECTION("binary /=")
    {
        vector lhs{2_q, 4_r};
        lhs /= 2;
        CHECK(lhs == vector{1_q, 2_r});
    }
    SECTION("norm")
    {
        STATIC_CHECK(vector{}.norm() == 0);
        STATIC_CHECK(vector{1_q, 2_r}.norm() == 3);
    }
    SECTION("distance")
    {
        STATIC_CHECK(distance({}, {}) == 0);
        STATIC_CHECK(distance({}, {1_q, -1_s}) == 1);
        STATIC_CHECK(distance({1_q, -1_s}, {}) == 1);
        STATIC_CHECK(distance({}, {2_q, -2_s}) == 2);
        STATIC_CHECK(distance({}, {-2_q, 0_s}) == 2);
        STATIC_CHECK(distance({}, {-2_q, -2_s}) == 4);
        STATIC_CHECK(distance({0_q, 4_r}, {-2_q, -2_r}) == 8);
    }
    SECTION("adjacent")
    {
        STATIC_CHECK(adjacent({}, {}) == false);
        STATIC_CHECK(adjacent({}, {1_q, -1_s}) == true);
        STATIC_CHECK(adjacent({}, {-1_q, 1_s}) == true);
        STATIC_CHECK(adjacent({-1_q, 1_s}, {}) == true);
        STATIC_CHECK(adjacent({-1_q, 1_s}, {-2_q, 1_s}) == true);
        STATIC_CHECK(adjacent({}, {2_q, -2_s}) == false);
        STATIC_CHECK(adjacent({}, {-2_q, 0_s}) == false);
        STATIC_CHECK(adjacent({}, {-2_q, -2_s}) == false);
    }
    SECTION("round")
    {
        using q_coord = q_coordinate<double>;
        using r_coord = r_coordinate<double>;
        STATIC_CHECK(round(vector{q_coord{0.0}, r_coord{0.0}}) == vector{});
        STATIC_CHECK(round(vector{q_coord{0.1}, r_coord{-0.4}}) == vector{});
    }
    SECTION("lerp")
    {
        using hex_vec = vector<double>;
        using q_coord = hex_vec::q_coordinate;
        using r_coord = hex_vec::r_coordinate;
        STATIC_CHECK(lerp(hex_vec{}, hex_vec{}, 0.) == lerp(hex_vec{}, hex_vec{}, 1.));
        STATIC_CHECK(lerp(hex_vec{}, hex_vec{q_coord{2}, r_coord{0}}, 0.) == hex_vec{});
        STATIC_CHECK(lerp(hex_vec{}, hex_vec{q_coord{2}, r_coord{0}}, 1.) == hex_vec{q_coord{2}, r_coord{0}});
        STATIC_CHECK(lerp(hex_vec{}, hex_vec{q_coord{2}, r_coord{0}}, 0.5) == hex_vec{q_coord{1}, r_coord{0}});
    }
    SECTION("rotate")
    {
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(0)) == vector{1_q, 0_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(1)) == vector{0_q, 1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(2)) == vector{-1_q, 1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(3)) == vector{-1_q, 0_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(4)) == vector{0_q, -1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(5)) == vector{1_q, -1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(6)) == vector{1_q, 0_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-1)) == vector{1_q, -1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-2)) == vector{0_q, -1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-3)) == vector{-1_q, 0_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-4)) == vector{-1_q, 1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-5)) == vector{0_q, 1_r});
        STATIC_CHECK(rotate(vector{1_q, 0_r}, rotation_steps(-6)) == vector{1_q, 0_r});
    }
    SECTION("reflect")
    {
        STATIC_CHECK(reflect(vector{}, coordinate_axis::q) == vector{});
        STATIC_CHECK(reflect(vector{}, coordinate_axis::r) == vector{});
        STATIC_CHECK(reflect(vector{}, coordinate_axis::s) == vector{});

        STATIC_CHECK(reflect(vector{0_q, -3_r}, coordinate_axis::q) == vector{0_q, 3_r});
        STATIC_CHECK(reflect(vector{0_q, -3_r}, coordinate_axis::r) == vector{3_q, -3_r});
        STATIC_CHECK(reflect(vector{0_q, -3_r}, coordinate_axis::s) == vector{-3_q, 0_r});
    }
    SECTION("unit vectors")
    {
        STATIC_CHECK(unit_qr.norm() == 1);
        STATIC_CHECK(unit_qs.norm() == 1);
        STATIC_CHECK(unit_rs.norm() == 1);
        STATIC_CHECK(unit_rq == -unit_qr);
        STATIC_CHECK(unit_sq == -unit_qs);
        STATIC_CHECK(unit_sr == -unit_rs);
        STATIC_CHECK(unit_qr != unit_qs);
        STATIC_CHECK(unit_qs != unit_rs);
        STATIC_CHECK(unit_rs != unit_qs);
    }
    SECTION("diagonal vectors")
    {
        STATIC_CHECK(diag_q.norm() == 2);
        STATIC_CHECK(diag_r.norm() == 2);
        STATIC_CHECK(diag_s.norm() == 2);
        STATIC_CHECK(diag_q.q().value() == 2);
        STATIC_CHECK(diag_r.r().value() == 2);
        STATIC_CHECK(diag_s.s().value() == 2);
    }
    SECTION("iterable")
    {
        STATIC_CHECK(std::random_access_iterator<vector<int>::iterator>);
        STATIC_CHECK(std::random_access_iterator<vector<int>::const_iterator>);
        STATIC_CHECK(std::random_access_iterator<vector<int>::reverse_iterator>);
        STATIC_CHECK(std::random_access_iterator<vector<int>::const_reverse_iterator>);

        constexpr vector v(1_q, 2_r);
        STATIC_CHECK(v.size() == 3);
        STATIC_CHECK(std::ranges::size(v) == 3);
        STATIC_CHECK(std::ranges::distance(v) == 3);
        STATIC_CHECK(v.end() - v.begin() == 3);

        STATIC_CHECK(v[0] == std::pair<coordinate_axis const, int>(coordinate_axis::q, v.q().value()));
        STATIC_CHECK(v[1] == std::pair<coordinate_axis const, int>(coordinate_axis::r, v.r().value()));
        STATIC_CHECK(v[2] == std::pair<coordinate_axis const, int>(coordinate_axis::s, v.s().value()));
        STATIC_CHECK(v[coordinate_axis::q] == v[0].second);
        STATIC_CHECK(v[coordinate_axis::r] == v[1].second);
        STATIC_CHECK(v[coordinate_axis::s] == v[2].second);

        for (auto&& [axis, value] : v)
            CHECK(v[axis] == value);

        for (auto&& [idx, axis] : v | std::views::keys | std::views::enumerate)
            CHECK(axis == coordinate_axis(idx));

        for (auto&& [idx, value] : v | std::views::values | std::views::enumerate)
            CHECK(v[idx].second == value);

        for (auto&& [axis, value] : v | std::views::reverse)
            CHECK(v[axis] == value);

        STATIC_CHECK(v.end() == vector<int>::iterator());
        STATIC_CHECK(v.begin() < v.end());
    }
}