cmake_minimum_required(VERSION 3.0.0)

 
project(EgoController VERSION 0.1.0)
 
 
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-unused-variable -pthread")
endif()
add_definitions(-DJSONCPP_USING_SECURE_MEMORY=0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations -Wno-deprecated-declarations    -pthread -g -Wall -Wno-unused-variable -DWEBRTC_POSIX=1 -DWEBRTC_LINUX=1")
 
 
add_definitions(-DJSONCPP_USING_SECURE_MEMORY=0)
set(API_DIR ../webrtcinterop/include)
 
include_directories(${API_DIR})
#find_package(librealsense2 REQUIRED)
#find_package(ZED REQUIRED)
#find_package(protobuf REQUIRED)
#find_package(jsoncpp REQUIRED) 


find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG glib-2.0)
if (GLIB_PKG_FOUND)
    message(Found glib-2.0)
endif (GLIB_PKG_FOUND)



message("${PROJECT_SOURCE_DIR}")
file(GLOB native_srcs "*.cpp")
set(CURDIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB common_srcs ${CURDIR}/../common/*.cpp)
 
 include_directories(${CURDIR}/../Protocol/linux/)
 file(GLOB proto_srcs ${CURDIR}/../Protocol/linux/*.cc)
 link_directories("../webrtcinterop/build")
 IF(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
 link_directories("../webrtc/lib/x64")
 elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
 link_directories("../webrtc/lib/aarch64")
 endif()
 
add_executable(EgoController ${native_srcs} ${common_srcs} ${proto_srcs})  
find_package(Boost COMPONENTS system REQUIRED)
message(Boost "${Boost_SYSTEM_LIBRARY}")

target_link_libraries(EgoController pthread)
target_link_libraries(EgoController libwebrtcinterop.a)
target_link_libraries(EgoController libwebrtc.a)

target_link_libraries(EgoController /usr/local/lib/libprotobuf.so)
target_link_libraries(EgoController libjsoncpp.so)
target_link_libraries(EgoController ${Boost_SYSTEM_LIBRARY})
target_link_libraries(EgoController dl)
target_link_libraries(EgoController X11)
target_link_libraries(EgoController  ${GLIB_LIBRARIES})

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})