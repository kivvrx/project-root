cmake_minimum_required(VERSION 3.10)
project(program2)

add_executable(program2 server.cpp)

target_include_directories(program2 PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/include
)

target_link_libraries(program2 PRIVATE 
    lib 
    Boost::system 
    Boost::asio
)

# Копирование lib.dll в папку с program2
add_custom_command(TARGET program2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:lib>
    $<TARGET_FILE_DIR:program2>
)