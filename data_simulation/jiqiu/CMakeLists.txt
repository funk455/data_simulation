cmake_minimum_required (VERSION 3.0)
project (view_modify)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/view_modify_data)

include_directories(${CMAKE_SOURCE_DIR}/json_config)

include_directories(${CMAKE_SOURCE_DIR}/json_config/cereal-master/include)

#add_library(./json_config libdev_json.a)
#生成库，可以在此基础上修改，例如后面换成cpp文件，但是已经有.a的库了，所以直接写最后链接一步。

# add_executable(view_modify view_modify_data/main.cpp view_modify_data/drv_set_query.cpp)

add_library(dev_view ${CMAKE_SOURCE_DIR}/jiqiu/view_modify_data/drv_set_query.cpp)

# target_link_libraries (view_modify ${CMAKE_SOURCE_DIR}/json_config/libdev_json.a)

#target_link_libraries (view_modify ${dev_json})