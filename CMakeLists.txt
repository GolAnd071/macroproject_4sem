cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(snowflake)

find_package(VTK REQUIRED)

set(SOURCE_FILES
    src/Main.cpp
    src/Mesh.cpp
    src/Params.cpp
    src/Point.cpp
    src/Simulation.cpp
    src/Utility.cpp
    )

set(INCLUDE_DIRS
    include/
    vendor/
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:ENABLE_PROFILING>
        _USE_MATH_DEFINES
)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
