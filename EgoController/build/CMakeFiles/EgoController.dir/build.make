# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zc/project/EgoZJ/EgoController

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zc/project/EgoZJ/EgoController/build

# Include any dependencies generated for this target.
include CMakeFiles/EgoController.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EgoController.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EgoController.dir/flags.make

CMakeFiles/EgoController.dir/PeerConnection.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/PeerConnection.cpp.o: ../PeerConnection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/EgoController.dir/PeerConnection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/PeerConnection.cpp.o -c /home/zc/project/EgoZJ/EgoController/PeerConnection.cpp

CMakeFiles/EgoController.dir/PeerConnection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/PeerConnection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/PeerConnection.cpp > CMakeFiles/EgoController.dir/PeerConnection.cpp.i

CMakeFiles/EgoController.dir/PeerConnection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/PeerConnection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/PeerConnection.cpp -o CMakeFiles/EgoController.dir/PeerConnection.cpp.s

CMakeFiles/EgoController.dir/PeerConnection.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/PeerConnection.cpp.o.requires

CMakeFiles/EgoController.dir/PeerConnection.cpp.o.provides: CMakeFiles/EgoController.dir/PeerConnection.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/PeerConnection.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/PeerConnection.cpp.o.provides

CMakeFiles/EgoController.dir/PeerConnection.cpp.o.provides.build: CMakeFiles/EgoController.dir/PeerConnection.cpp.o


CMakeFiles/EgoController.dir/VideoRenderer.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/VideoRenderer.cpp.o: ../VideoRenderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/EgoController.dir/VideoRenderer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/VideoRenderer.cpp.o -c /home/zc/project/EgoZJ/EgoController/VideoRenderer.cpp

CMakeFiles/EgoController.dir/VideoRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/VideoRenderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/VideoRenderer.cpp > CMakeFiles/EgoController.dir/VideoRenderer.cpp.i

CMakeFiles/EgoController.dir/VideoRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/VideoRenderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/VideoRenderer.cpp -o CMakeFiles/EgoController.dir/VideoRenderer.cpp.s

CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.requires

CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.provides: CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.provides

CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.provides.build: CMakeFiles/EgoController.dir/VideoRenderer.cpp.o


CMakeFiles/EgoController.dir/asyncio_utils.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/asyncio_utils.cpp.o: ../asyncio_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/EgoController.dir/asyncio_utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/asyncio_utils.cpp.o -c /home/zc/project/EgoZJ/EgoController/asyncio_utils.cpp

CMakeFiles/EgoController.dir/asyncio_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/asyncio_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/asyncio_utils.cpp > CMakeFiles/EgoController.dir/asyncio_utils.cpp.i

CMakeFiles/EgoController.dir/asyncio_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/asyncio_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/asyncio_utils.cpp -o CMakeFiles/EgoController.dir/asyncio_utils.cpp.s

CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.requires

CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.provides: CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.provides

CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.provides.build: CMakeFiles/EgoController.dir/asyncio_utils.cpp.o


CMakeFiles/EgoController.dir/main.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/EgoController.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/main.cpp.o -c /home/zc/project/EgoZJ/EgoController/main.cpp

CMakeFiles/EgoController.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/main.cpp > CMakeFiles/EgoController.dir/main.cpp.i

CMakeFiles/EgoController.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/main.cpp -o CMakeFiles/EgoController.dir/main.cpp.s

CMakeFiles/EgoController.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/main.cpp.o.requires

CMakeFiles/EgoController.dir/main.cpp.o.provides: CMakeFiles/EgoController.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/main.cpp.o.provides

CMakeFiles/EgoController.dir/main.cpp.o.provides.build: CMakeFiles/EgoController.dir/main.cpp.o


CMakeFiles/EgoController.dir/message_queue.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/message_queue.cpp.o: ../message_queue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/EgoController.dir/message_queue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/message_queue.cpp.o -c /home/zc/project/EgoZJ/EgoController/message_queue.cpp

CMakeFiles/EgoController.dir/message_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/message_queue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/message_queue.cpp > CMakeFiles/EgoController.dir/message_queue.cpp.i

CMakeFiles/EgoController.dir/message_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/message_queue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/message_queue.cpp -o CMakeFiles/EgoController.dir/message_queue.cpp.s

CMakeFiles/EgoController.dir/message_queue.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/message_queue.cpp.o.requires

CMakeFiles/EgoController.dir/message_queue.cpp.o.provides: CMakeFiles/EgoController.dir/message_queue.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/message_queue.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/message_queue.cpp.o.provides

CMakeFiles/EgoController.dir/message_queue.cpp.o.provides.build: CMakeFiles/EgoController.dir/message_queue.cpp.o


CMakeFiles/EgoController.dir/socket_lidar.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/socket_lidar.cpp.o: ../socket_lidar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/EgoController.dir/socket_lidar.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/socket_lidar.cpp.o -c /home/zc/project/EgoZJ/EgoController/socket_lidar.cpp

CMakeFiles/EgoController.dir/socket_lidar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/socket_lidar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/EgoController/socket_lidar.cpp > CMakeFiles/EgoController.dir/socket_lidar.cpp.i

CMakeFiles/EgoController.dir/socket_lidar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/socket_lidar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/EgoController/socket_lidar.cpp -o CMakeFiles/EgoController.dir/socket_lidar.cpp.s

CMakeFiles/EgoController.dir/socket_lidar.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/socket_lidar.cpp.o.requires

CMakeFiles/EgoController.dir/socket_lidar.cpp.o.provides: CMakeFiles/EgoController.dir/socket_lidar.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/socket_lidar.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/socket_lidar.cpp.o.provides

CMakeFiles/EgoController.dir/socket_lidar.cpp.o.provides.build: CMakeFiles/EgoController.dir/socket_lidar.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o: /home/zc/project/EgoZJ/common/Feedback.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o -c /home/zc/project/EgoZJ/common/Feedback.cpp

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/common/Feedback.cpp > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/common/Feedback.cpp -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o: /home/zc/project/EgoZJ/common/iobuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o -c /home/zc/project/EgoZJ/common/iobuffer.cpp

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/common/iobuffer.cpp > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/common/iobuffer.cpp -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o: /home/zc/project/EgoZJ/common/peer_connection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o -c /home/zc/project/EgoZJ/common/peer_connection.cpp

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/common/peer_connection.cpp > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/common/peer_connection.cpp -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o: /home/zc/project/EgoZJ/common/socket_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o -c /home/zc/project/EgoZJ/common/socket_client.cpp

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/common/socket_client.cpp > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/common/socket_client.cpp -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o: /home/zc/project/EgoZJ/common/update_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o -c /home/zc/project/EgoZJ/common/update_thread.cpp

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/common/update_thread.cpp > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/common/update_thread.cpp -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o


CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o: CMakeFiles/EgoController.dir/flags.make
CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o: /home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o -c /home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc > CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.i

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc -o CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.s

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.requires:

.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.requires

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.provides: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/EgoController.dir/build.make CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.provides.build
.PHONY : CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.provides

CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.provides.build: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o


# Object files for target EgoController
EgoController_OBJECTS = \
"CMakeFiles/EgoController.dir/PeerConnection.cpp.o" \
"CMakeFiles/EgoController.dir/VideoRenderer.cpp.o" \
"CMakeFiles/EgoController.dir/asyncio_utils.cpp.o" \
"CMakeFiles/EgoController.dir/main.cpp.o" \
"CMakeFiles/EgoController.dir/message_queue.cpp.o" \
"CMakeFiles/EgoController.dir/socket_lidar.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o" \
"CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o"

# External object files for target EgoController
EgoController_EXTERNAL_OBJECTS =

EgoController: CMakeFiles/EgoController.dir/PeerConnection.cpp.o
EgoController: CMakeFiles/EgoController.dir/VideoRenderer.cpp.o
EgoController: CMakeFiles/EgoController.dir/asyncio_utils.cpp.o
EgoController: CMakeFiles/EgoController.dir/main.cpp.o
EgoController: CMakeFiles/EgoController.dir/message_queue.cpp.o
EgoController: CMakeFiles/EgoController.dir/socket_lidar.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o
EgoController: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o
EgoController: CMakeFiles/EgoController.dir/build.make
EgoController: /usr/local/lib/libprotobuf.so
EgoController: /usr/lib/aarch64-linux-gnu/libboost_system.so
EgoController: CMakeFiles/EgoController.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zc/project/EgoZJ/EgoController/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable EgoController"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EgoController.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EgoController.dir/build: EgoController

.PHONY : CMakeFiles/EgoController.dir/build

CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/PeerConnection.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/VideoRenderer.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/asyncio_utils.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/main.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/message_queue.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/socket_lidar.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/Feedback.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/iobuffer.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/peer_connection.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/socket_client.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/common/update_thread.cpp.o.requires
CMakeFiles/EgoController.dir/requires: CMakeFiles/EgoController.dir/home/zc/project/EgoZJ/Protocol/linux/protocol.pb.cc.o.requires

.PHONY : CMakeFiles/EgoController.dir/requires

CMakeFiles/EgoController.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EgoController.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EgoController.dir/clean

CMakeFiles/EgoController.dir/depend:
	cd /home/zc/project/EgoZJ/EgoController/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zc/project/EgoZJ/EgoController /home/zc/project/EgoZJ/EgoController /home/zc/project/EgoZJ/EgoController/build /home/zc/project/EgoZJ/EgoController/build /home/zc/project/EgoZJ/EgoController/build/CMakeFiles/EgoController.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EgoController.dir/depend

