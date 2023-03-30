cmake_minimum_required(VERSION 3.0.0)
project(updator VERSION 0.1.0)

 
#set(CMAKE_CXX_FLAGS "-D_USE_UTF8")
FILE(GLOB LIVEHELPERSOURCE *.c *.cpp)
add_executable(${PROJECT_NAME} ${LIVEHELPERSOURCE})
target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} libjsoncpp.so)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
 
