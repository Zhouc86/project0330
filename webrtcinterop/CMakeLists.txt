cmake_minimum_required(VERSION 3.0.0)
project(webrtcinterop VERSION 0.1.0)
MESSAGE(STATUS "operation system is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
IF(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    SET (CMAKE_C_COMPILER             "/usr/bin/clang")
    SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
    SET (CMAKE_AR      "/usr/bin/llvm-ar")
    SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
   SET (CMAKE_NM      "/usr/bin/llvm-nm")
    SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
ENDIF()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g2 -ggdb -Wall -Wno-unused-variable -pthread")
endif()
 
#find_package(CUDA REQUIRED)


 
add_definitions(-DWEBRTC_POSIX)
add_definitions(-DWEBRTC_LINUX)
add_definitions(-DJSONCPP_USING_SECURE_MEMORY=0)
set(API_DIR ./include)
   
set(WEBRTC_DIR ../thirdparty/webrtc)
include_directories(${API_DIR})
include_directories(${WEBRTC_DIR}/include)
include_directories(${WEBRTC_DIR}/include/third_party/abseil-cpp)
include_directories(${WEBRTC_DIR}/include/third_party/libyuv/include)
include_directories(${WEBRTC_DIR}/include/third_party/jsoncpp/source/include)
include_directories(${WEBRTC_DIR}/include/third_party/jsoncpp/generated)


#if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
#add_definitions(-DWEBRTC_AARCH)
#include_directories(${WEBRTC_DIR}/include/aarch64)
#nclude_directories(${WEBRTC_DIR}/include/aarch64/third_party/abseil-cpp)
#include_directories(${WEBRTC_DIR}/include/aarch64/third_party/libyuv/include)
#include_directories(${WEBRTC_DIR}/include/aarch64/third_party/jsoncpp/source/include)
#include_directories(${WEBRTC_DIR}/include/aarch64/third_party/jsoncpp/generated)
#endif()
#include_directories(${CUDA_INCLUDE_DIRS}
#    ${ZED_INCLUDE_DIRS}
#     )
#set(LIVE ${CMAKE_BINARY_DIR}/../live)
#message(${LIVE})
FILE(GLOB LIVEHELPERSOURCE  src/*.cpp *.cpp)
 
add_library(webrtcinterop ${LIVEHELPERSOURCE})
#target_include_directories(webrtcinterop PUBLIC ${LIVE}/groupsock/include ${LIVE}/liveMedia/include ${LIVE}/UsageEnvironment/include ${LIVE}/BasicUsageEnvironment/include inc)
target_compile_definitions(webrtcinterop PUBLIC NO_OPENSSL=1 BSD=1 SOCKLEN_T=socklen_t _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE=1)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
