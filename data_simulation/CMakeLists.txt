cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

project(SIM_ROOT)

# 自动生成compile_commands.json
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions("-lpthread")

add_subdirectory(player)

add_subdirectory(jiqiu)

set(dev_json "${CMAKE_SOURCE_DIR}/json_config/libdev_json.a")

set(dev_player "${CMAKE_SOURCE_DIR}/build/player/libdev_pla.a")

set(dev_view "${CMAKE_SOURCE_DIR}/build/jiqiu/libdev_view.a")

include_directories(
    ${CMAKE_SOURCE_DIR}/json_config/cereal-master/include
    ${CMAKE_SOURCE_DIR}/json_config
    ${CMAKE_SOURCE_DIR}/fd_main/include
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/jiqiu/view_modify_data
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# test case or example
add_subdirectory("./fd_main")
# add_subdirectory("./json_config")
# add_subdirectory("./player")

