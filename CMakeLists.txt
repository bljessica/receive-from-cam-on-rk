cmake_minimum_required(VERSION 3.10.2) # 设定版本

set(CMAKE_PROJECT_NAME receive_from_cam_on_rk)
project(${CMAKE_PROJECT_NAME}) # 设定项目名

SET(PROJECT_ROOT_PATH ${PROJECT_SOURCE_DIR})

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_C_COMPILER ${PROJECT_ROOT_PATH}/../rk/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${PROJECT_ROOT_PATH}/../rk/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

    
# 查找路径下的所有源文件（因为头文件和实现分离，所以要把所有源文件都添加进来）
aux_source_directory(${PROJECT_ROOT_PATH}/src DIR_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_PATH}/bin)

# 生成程序
add_executable(${CMAKE_PROJECT_NAME} ${DIR_SRCS})
