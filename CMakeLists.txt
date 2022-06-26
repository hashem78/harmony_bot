cmake_minimum_required(VERSION 3.0.0)
project(harmony VERSION 0.1.0)

add_executable( harmony
src/main.cpp
src/harmony_commands.cpp
src/indexing_context.cpp
src/harmony_config.cpp
src/include/harmony_commands.hpp
src/include/indexing_context.hpp
src/include/harmony_config.hpp)

add_subdirectory(libs/DPP)
add_subdirectory(libs/fmt)
add_subdirectory(libs/nlohmann/json)
add_subdirectory(libs/re2)

target_link_libraries(harmony dpp)
target_link_libraries(harmony re2)
target_link_libraries(harmony fmt::fmt-header-only)
target_link_libraries(harmony nlohmann_json::nlohmann_json)

target_include_directories(harmony PRIVATE
    libs/DPP/include
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(BUILD_SHARED_LIBS TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

