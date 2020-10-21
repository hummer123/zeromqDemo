cmake_minimum_required(VERSION 3.10)

project(demoMQ)

# 设置编译器选项：O3优化，显示所有警告
set(CMAKE_C_FLAGS "$ENV{CFLAGS} -O1 -Wall -g -fPIC")


ADD_SUBDIRECTORY(src)

