cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_FLAGS "-std=c++17 -g")

project(snowflake)
add_executable(main main.cpp)

# Найдём VTK
find_package(VTK REQUIRED)
# ... и подключим её со всеми её компонентами её нативными средствами
include(${VTK_USE_FILE})

# Укажем библиотеки для линковки
target_link_libraries(main ${VTK_LIBRARIES})
