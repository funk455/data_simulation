CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)
PROJECT(simulation)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED true)

include_directories( ${PROJECT_SOURCE_DIR} ) #增加头文件

include_directories(${CMAKE_SOURCE_DIR}/json_config)
include_directories(${CMAKE_SOURCE_DIR}/json_config/cereal-master/include)
include_directories(${CMAKE_SOURCE_DIR}/fd_main/include)
include_directories(${CMAKE_SOURCE_DIR}/jiqiu/view_modify_data)

add_definitions() #增加宏定义
#.c
SET(RCS_SRC 
${PROJECT_SOURCE_DIR}/canplayer.c
${PROJECT_SOURCE_DIR}/lib.c
)
#.cpp
set(SRCC
${PROJECT_SOURCE_DIR}/pty.cpp
${PROJECT_SOURCE_DIR}/player.cpp
${PROJECT_SOURCE_DIR}/filename.cpp)
#main
#FILE(GLOB_RECURSE MAIN_LIST ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# ADD_EXECUTABLE(${PROJECT_NAME}  ${RCS_SRC}   ${SRCC}
#          )

add_library(dev_pla ${RCS_SRC} ${SRCC})

