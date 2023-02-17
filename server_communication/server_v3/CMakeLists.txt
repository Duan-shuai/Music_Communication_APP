cmake_minimum_required(VERSION 3.2)

project(server_v3)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 第三方库默认头文件位置：/usr/local/include

# mysql头文件位置
include_directories(/usr/include/mysql)

# 第三方库默认库文件位置：/usr/local/lib

# mysql库文件位置
link_directories(/usr/lib/mysql)

add_executable(${PROJECT_NAME} ${DIR_SRCS} server_v3.cpp)
target_link_libraries(${PROJECT_NAME} libjsoncpp.a)
target_link_libraries(${PROJECT_NAME} libmysqlclient.so)