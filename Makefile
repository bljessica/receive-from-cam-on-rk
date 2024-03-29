# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/dyh/Documents/receive-from-cam-on-rk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dyh/Documents/receive-from-cam-on-rk

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/cmake-3.22.1/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dyh/Documents/receive-from-cam-on-rk/CMakeFiles /home/dyh/Documents/receive-from-cam-on-rk/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dyh/Documents/receive-from-cam-on-rk/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named receive_from_cam_on_rk

# Build rule for target.
receive_from_cam_on_rk: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 receive_from_cam_on_rk
.PHONY : receive_from_cam_on_rk

# fast build rule for target.
receive_from_cam_on_rk/fast:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/build
.PHONY : receive_from_cam_on_rk/fast

src/Decoder.o: src/Decoder.cpp.o

.PHONY : src/Decoder.o

# target to build an object file
src/Decoder.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/Decoder.cpp.o
.PHONY : src/Decoder.cpp.o

src/Decoder.i: src/Decoder.cpp.i

.PHONY : src/Decoder.i

# target to preprocess a source file
src/Decoder.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/Decoder.cpp.i
.PHONY : src/Decoder.cpp.i

src/Decoder.s: src/Decoder.cpp.s

.PHONY : src/Decoder.s

# target to generate assembly for a file
src/Decoder.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/Decoder.cpp.s
.PHONY : src/Decoder.cpp.s

src/SocketClient.o: src/SocketClient.cpp.o

.PHONY : src/SocketClient.o

# target to build an object file
src/SocketClient.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/SocketClient.cpp.o
.PHONY : src/SocketClient.cpp.o

src/SocketClient.i: src/SocketClient.cpp.i

.PHONY : src/SocketClient.i

# target to preprocess a source file
src/SocketClient.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/SocketClient.cpp.i
.PHONY : src/SocketClient.cpp.i

src/SocketClient.s: src/SocketClient.cpp.s

.PHONY : src/SocketClient.s

# target to generate assembly for a file
src/SocketClient.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/SocketClient.cpp.s
.PHONY : src/SocketClient.cpp.s

src/cJSON.o: src/cJSON.c.o

.PHONY : src/cJSON.o

# target to build an object file
src/cJSON.c.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/cJSON.c.o
.PHONY : src/cJSON.c.o

src/cJSON.i: src/cJSON.c.i

.PHONY : src/cJSON.i

# target to preprocess a source file
src/cJSON.c.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/cJSON.c.i
.PHONY : src/cJSON.c.i

src/cJSON.s: src/cJSON.c.s

.PHONY : src/cJSON.s

# target to generate assembly for a file
src/cJSON.c.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/cJSON.c.s
.PHONY : src/cJSON.c.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/read_camera.o: src/read_camera.cpp.o

.PHONY : src/read_camera.o

# target to build an object file
src/read_camera.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/read_camera.cpp.o
.PHONY : src/read_camera.cpp.o

src/read_camera.i: src/read_camera.cpp.i

.PHONY : src/read_camera.i

# target to preprocess a source file
src/read_camera.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/read_camera.cpp.i
.PHONY : src/read_camera.cpp.i

src/read_camera.s: src/read_camera.cpp.s

.PHONY : src/read_camera.s

# target to generate assembly for a file
src/read_camera.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/read_camera.cpp.s
.PHONY : src/read_camera.cpp.s

src/rockface_ctl.o: src/rockface_ctl.cpp.o

.PHONY : src/rockface_ctl.o

# target to build an object file
src/rockface_ctl.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/rockface_ctl.cpp.o
.PHONY : src/rockface_ctl.cpp.o

src/rockface_ctl.i: src/rockface_ctl.cpp.i

.PHONY : src/rockface_ctl.i

# target to preprocess a source file
src/rockface_ctl.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/rockface_ctl.cpp.i
.PHONY : src/rockface_ctl.cpp.i

src/rockface_ctl.s: src/rockface_ctl.cpp.s

.PHONY : src/rockface_ctl.s

# target to generate assembly for a file
src/rockface_ctl.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/rockface_ctl.cpp.s
.PHONY : src/rockface_ctl.cpp.s

src/util.o: src/util.cpp.o

.PHONY : src/util.o

# target to build an object file
src/util.cpp.o:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/util.cpp.o
.PHONY : src/util.cpp.o

src/util.i: src/util.cpp.i

.PHONY : src/util.i

# target to preprocess a source file
src/util.cpp.i:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/util.cpp.i
.PHONY : src/util.cpp.i

src/util.s: src/util.cpp.s

.PHONY : src/util.s

# target to generate assembly for a file
src/util.cpp.s:
	$(MAKE) -f CMakeFiles/receive_from_cam_on_rk.dir/build.make CMakeFiles/receive_from_cam_on_rk.dir/src/util.cpp.s
.PHONY : src/util.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... receive_from_cam_on_rk"
	@echo "... edit_cache"
	@echo "... src/Decoder.o"
	@echo "... src/Decoder.i"
	@echo "... src/Decoder.s"
	@echo "... src/SocketClient.o"
	@echo "... src/SocketClient.i"
	@echo "... src/SocketClient.s"
	@echo "... src/cJSON.o"
	@echo "... src/cJSON.i"
	@echo "... src/cJSON.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/read_camera.o"
	@echo "... src/read_camera.i"
	@echo "... src/read_camera.s"
	@echo "... src/rockface_ctl.o"
	@echo "... src/rockface_ctl.i"
	@echo "... src/rockface_ctl.s"
	@echo "... src/util.o"
	@echo "... src/util.i"
	@echo "... src/util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

