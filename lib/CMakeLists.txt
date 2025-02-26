cmake_minimum_required(VERSION 3.10)
project(lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib SHARED src/lib.cpp)

target_include_directories(lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Настройка видимости символов для Linux
set_target_properties(lib PROPERTIES
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON
)