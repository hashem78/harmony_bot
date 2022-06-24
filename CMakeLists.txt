cmake_minimum_required(VERSION 3.0.0)
project(harmony VERSION 0.1.0)

add_executable(harmony main.cpp)

add_subdirectory(libs/DPP)
add_subdirectory(libs/fmt)
add_subdirectory(libs/nlohmann/json)

target_link_libraries(harmony dpp)
target_link_libraries(harmony fmt::fmt)
target_link_libraries(harmony nlohmann_json::nlohmann_json)

target_include_directories(harmony PRIVATE
    libs/DPP/include
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

