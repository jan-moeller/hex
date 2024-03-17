#
# MIT License
#
# Copyright (c) 2024 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.14...3.22)

set(LIB_UNDER_TEST hex)
project(${LIB_UNDER_TEST}-tests LANGUAGES CXX)

#############################################################################################################
# Build configuration
#############################################################################################################
option(HEX_ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(HEX_TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)
option(HEX_USE_ASAN "Enable address sanitizer in the test build" OFF)

message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")
message(STATUS "Test coverage:             ${HEX_ENABLE_TEST_COVERAGE}")
message(STATUS "Test installed:            ${HEX_TEST_INSTALLED_VERSION}")
message(STATUS "Use address sanitizer      ${HEX_USE_ASAN}")

#############################################################################################################
# Dependencies
#############################################################################################################
include(../cmake/CPM.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.5.1")

# Find library under test
if (HEX_TEST_INSTALLED_VERSION)
    find_package(${LIB_UNDER_TEST} REQUIRED)
else ()
    if (NOT TARGET ${LIB_UNDER_TEST})
        CPMAddPackage(NAME ${LIB_UNDER_TEST} SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
    endif ()
endif ()

#############################################################################################################
# Main test target
#############################################################################################################

add_executable(${PROJECT_NAME}
        src/detail/test_sqrt.cpp
        src/grid/test_grid.cpp
        src/vector/test_coordinate.cpp
        src/vector/test_vector.cpp
        src/views/convex_polygon/detail/test_convex_polygon_iterator.cpp
        src/views/convex_polygon/detail/test_hexagon_size.cpp
        src/views/convex_polygon/detail/test_isosceles_trapezoid_size.cpp
        src/views/convex_polygon/test_convex_polygon_parameters.cpp
        src/views/convex_polygon/test_convex_polygon_view.cpp
        src/views/line/detail/test_line_iterator.cpp
        src/views/line/test_line_view.cpp
        src/views/neighbors/test_neighbors_view.cpp
        src/views/offset_rows/detail/test_offset_conversion.cpp
        src/views/offset_rows/test_offset_rows_view.cpp
        src/vector/test_vector.cpp
)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain ${LIB_UNDER_TEST}::${LIB_UNDER_TEST})

# enable compiler warnings
if (NOT HEX_TEST_INSTALLED_VERSION)
    set_target_properties(${LIB_UNDER_TEST} ${PROJECT_NAME} PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            COMPILE_WARNING_AS_ERROR ON
    )
    target_compile_options(${LIB_UNDER_TEST} INTERFACE
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
            $<$<CXX_COMPILER_ID:GNU>:-Wno-maybe-uninitialized> # -Wmaybe-uninitialized has false positives with -O3
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
    target_compile_options(${PROJECT_NAME} PUBLIC
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
            $<$<CXX_COMPILER_ID:GNU>:-Wno-maybe-uninitialized> # -Wmaybe-uninitialized has false positives with -O3
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )

    if (HEX_USE_ASAN)
        target_compile_options(${LIB_UNDER_TEST} INTERFACE
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address>
        )
        target_link_options(${LIB_UNDER_TEST} INTERFACE
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address>
        )
        target_compile_options(${PROJECT_NAME} PUBLIC
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address>
        )
        target_link_options(${PROJECT_NAME} PUBLIC
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address>
        )
    endif ()
endif ()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
enable_testing()

catch_discover_tests(${PROJECT_NAME})

if (HEX_ENABLE_TEST_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(${PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
endif ()