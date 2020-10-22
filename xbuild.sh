#!/bin/sh

#set -x
set -e

PROJECT_DIR=$PWD
EXEC_DIR=$PWD/bin
EXEC_SYM=$PWD/symbol
PROJ_BUILD=$PWD/build
PROJ_UTILS_CZMQ=$PWD/utils/czmq
PROJ_UTILS_LIBZMQ=$PWD/utils/libzmq

CROSS_COMPILE=""
export STRIP=$CROSS_COMPILE"strip"
export OBJCOPY=$CROSS_COMPILE"objcopy"

echo "===> STRIP:   $STRIP"
echo "===> OBJCOPY: $OBJCOPY"

# 输出调试符号表
BUILD_LIB_OutputSymbol()
{
    INPUT_PATH=$1
    OUTPUT_PATH=$2

    for i in `ls -X $INPUT_PATH`
    do
        $OBJCOPY --only-keep-debug $INPUT_PATH/$i $OUTPUT_PATH/$i.sym
        $STRIP $INPUT_PATH/$i
        $STRIP --remove-section=.comment $INPUT_PATH/$i
		$OBJCOPY --add-gnu-debuglink=$OUTPUT_PATH/$i.sym $INPUT_PATH/$i
    done
}

# 清空原安装目录
rm $EXEC_DIR -rf
rm $EXEC_SYM -rf
rm $PROJ_BUILD -rf

mkdir $EXEC_DIR
mkdir $EXEC_SYM
mkdir $PROJ_BUILD

# cmake 编译工程
cd ${PROJ_BUILD}
cmake ..
make

# 将工程提供的动态库或可执行程序符号表剔除
BUILD_LIB_OutputSymbol $EXEC_DIR          $EXEC_SYM

# !!! 因调试期间工程会多次编译，所以这里动态库暂时不剔符号表
# BUILD_LIB_OutputSymbol $PROJ_UTILS_CZMQ   $EXEC_SYM
# BUILD_LIB_OutputSymbol $PROJ_UTILS_LIBZMQ $EXEC_SYM
