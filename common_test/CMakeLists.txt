set(CMAKE_AUTOMOC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)

add_executable(common_test main.cpp)
target_link_libraries(common_test LINK_PUBLIC Common)
