cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Указать путь к vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg-master/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Найти Boost
find_package(Boost REQUIRED COMPONENTS system asio)

add_subdirectory(lib)
add_subdirectory(program1)
add_subdirectory(program2)