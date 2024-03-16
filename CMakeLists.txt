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

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(hex
        VERSION 0.0.0
        LANGUAGES CXX
)

#############################################################################################################
# Prevent in-source builds
#############################################################################################################
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

#############################################################################################################
# Dependencies
#############################################################################################################
include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.1")

#############################################################################################################
# Build configuration
#############################################################################################################
option(HEX_BUILD_TESTS "Include test target in main build (prefer running a standalone test build from the test/ directory)" OFF)

message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")
message(STATUS "Build tests:               ${HEX_BUILD_TESTS}")

#############################################################################################################
# Main target
#############################################################################################################
add_library(${PROJECT_NAME} INTERFACE
        include/hex/detail/detail_arithmetic.hpp
        include/hex/detail/detail_generating_random_access_iterator.hpp
        include/hex/detail/detail_narrowing.hpp
        include/hex/detail/detail_parse_integer_literal.hpp
        include/hex/detail/detail_sqrt.hpp
        include/hex/grid/detail/detail_grid_iterator.hpp
        include/hex/grid/grid.hpp
        include/hex/hex.hpp
        include/hex/vector/coordinate.hpp
        include/hex/vector/coordinate_axis.hpp
        include/hex/vector/detail/detail_vector_iterator.hpp
        include/hex/vector/rotation_steps.hpp
        include/hex/vector/vector.hpp
        include/hex/views/convex_polygon/convex_polygon_parameters.hpp
        include/hex/views/convex_polygon/convex_polygon_view.hpp
        include/hex/views/convex_polygon/detail/detail_convex_polygon_iterator.hpp
        include/hex/views/convex_polygon/detail/detail_hexagon_size.hpp
        include/hex/views/convex_polygon/detail/detail_isosceles_trapezoid_size.hpp
        include/hex/views/neighbors/detail/detail_neighbors.hpp
        include/hex/views/neighbors/neighbors_view.hpp
        include/hex/views/offset_rows/detail/detail_offset_conversion.hpp
        include/hex/views/offset_rows/offset_parity.hpp
        include/hex/views/offset_rows/offset_rows_parameters.hpp
        include/hex/views/offset_rows/offset_rows_view.hpp
)
target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_compile_options(${PROJECT_NAME} INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES ""
)

if (${HEX_BUILD_TESTS})
    add_subdirectory(test)
endif ()
