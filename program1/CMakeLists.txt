cmake_minimum_required(VERSION 3.10)
project(program1)

add_executable(program1 client.cpp)

target_include_directories(program1 PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/include
)

target_link_libraries(program1 PRIVATE 
    lib 
    Boost::system 
    Boost::asio
)

# Копирование lib.dll в папку с program1
add_custom_command(TARGET program1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:lib>
    $<TARGET_FILE_DIR:program1>
)