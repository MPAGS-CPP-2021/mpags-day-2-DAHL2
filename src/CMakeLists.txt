# Minimum requirements
cmake_minimum_required(VERSION 3.2)

# Declare project
project(MPAGSCipher VERSION 0.1.0)

# Set info text to be output when using make
set(CMAKE_VERBOSE_MAKEFILE ON)

# Declare the executable files
add_executable(mpags-cipher
    mpags-cipher.cpp
    MPAGSCipher/TransformChar.cpp
    )

# Directory with project dependancies
target_include_directories(mpags-cipher
    PRIVATE MPAGSCipher
    )

# Add additional flags to compiling
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -Wshadow -pedantic")

# Set the requirements on C++
target_compile_features(mpags-cipher
    PRIVATE cxx_auto_type cxx_range_for cxx_uniform_initialization
    )

# Disallowed Vendor Extensions
set(CMAKE_CXX_EXTENSIONS OFF)