cmake_minimum_required(VERSION 3.0.0)

 
project(EgoSystem VERSION 1.0.0)
 
set(USE_LIDAR  OFF) 
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
add_definitions(-DJSONCPP_USING_SECURE_MEMORY=0)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations -Wno-deprecated-declarations    -pthread -g -Wall -Wno-unused-variable -DWEBRTC_POSIX=1 -DWEBRTC_LINUX=1")
 

 
add_definitions(-DWEBRTC_POSIX=1)
add_definitions(-DWEBRTC_LINUX=1)
 
set(API_DIR ../webrtcinterop/include)
 
include_directories(${API_DIR})
 
#find_package(protobuf REQUIRED)
 


find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG glib-2.0)
if (GLIB_PKG_FOUND)
    message(Found glib-2.0)
endif (GLIB_PKG_FOUND)

#pkg_check_modules(PC_LIBUDEV libudev)
#if(PC_LIBUDEV_FOUND)
#    message(Found libudev)
#endif(PC_LIBUDEV_FOUND)
#find_package(protobuf REQUIRED)
find_path(MODBUS_INCLUDE_DIR NAMES modbus.h modbus/modbus.h REQUIRED)
message(STATUS "MODBUS_INCLUDE_DIR: ${MODBUS_INCLUDE_DIR}")
   
message("${protobuf_INCLUDE_DIRS}")
file(GLOB native_srcs "*.cpp")
set(CURDIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB common_srcs ${CURDIR}/../common/*.cpp)
include_directories(${protobuf_INCLUDE_DIRS})
set(RS_DRIVER ${CURDIR}/../thirdparty/rs_driver)
message(RS_DRIVER "${RS_DRIVER}")
include_directories(${RS_DRIVER})
include_directories(${CURDIR}/../thirdparty/protobuf/include)
include_directories(${CURDIR}/../Protocol/linux/)
file(GLOB proto_srcs ${CURDIR}/../Protocol/linux/*.cc)
file(GLOB lidar_srcs ${CURDIR}/lidar/*.cpp)
link_directories("../webrtcinterop/build")
IF(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
link_directories("../thirdparty/webrtc/lib/linux")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
link_directories("../thirdparty/webrtc/lib/aarch64")
link_directories("../thirdparty/protobuf/lib/aarch64")
endif()

#message(lidar "${lidar_srcs}")
add_executable(EgoSystem ${native_srcs} ${common_srcs} ${proto_srcs})
 
find_package(Boost COMPONENTS system REQUIRED)
message(Boost "${Boost_SYSTEM_LIBRARY}")
#find_library(
#    UDEV_LIB
#    udev
#)
 
#list(APPEND EXTERNAL_LIBS pcap)
list(APPEND EXTERNAL_LIBS "-lpthread")
#list(APPEND EXTERNAL_LIBS pcap)
list(APPEND EXTERNAL_LIBS "libprotobuf.a")
#target_link_libraries(EgoSystem pthread)
target_link_libraries(EgoSystem libwebrtcinterop.a)
target_link_libraries(EgoSystem libwebrtc.a)
target_link_libraries(EgoSystem  ${EXTERNAL_LIBS} )
#target_link_libraries(EgoSystem /usr/local/lib/libprotobuf.so)
target_link_libraries(EgoSystem libjsoncpp.so)
target_link_libraries(EgoSystem ${Boost_SYSTEM_LIBRARY})
target_link_libraries(EgoSystem dl)
target_link_libraries(EgoSystem X11)
target_link_libraries(EgoSystem  ${GLIB_LIBRARIES})
#target_link_libraries(EgoSystem  ${PC_LIBUDEV_LIBRARIES})
#target_link_libraries(EgoSystem  ${UDEV_LIB})
#find_library(MODBUS_LIBRARY NAMES modbus REQUIRED)
#target_link_libraries(EgoSystem  ${MODBUS_LIBRARY})
 