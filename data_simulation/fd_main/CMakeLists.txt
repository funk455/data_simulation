cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_C_STANDARD 11)
# SET(CMAKE_C_STANDARD_REQUIRED true)

project(MAIN)

# add_subdirectory(../player)

# include_directories(${CMAKE_SOURCE_DIR}/../json_config)
# include_directories(${PROJECT_SOURCE_DIR}/../json_config/cereal-master/include)
# include_directories(${CMAKE_SOURCE_DIR}/../player)
# include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${PROJECT_SOURCE_DIR}/../player)

add_executable(main
        ./fd_main.cpp main.cpp
)
# set(dev_json ${CMAKE_SOURCE_DIR}/../json_config/libdev_json.a)

target_link_libraries(main ${dev_json} ${dev_player} ${dev_view} -pthread)

