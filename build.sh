#!/bin/bash
set -e

usage() {
  echo "Usage: $0 [BUILD_TYPE] [BUILD_STATIC]"
  echo "  BUILD_TYPE: Debug, Release"
  echo "  BUILD_STATIC: ON, OFF"
  exit 1
}

BUILD_TYPE=${1:-"Release"}
BUILD_STATIC=${2:-"ON"}

# 获取绝对路径
ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

# 创建build目录
BUILD_DIR=${ROOT_PWD}/build
if [[ -d "${BUILD_DIR}" ]]; then
  rm -rf ${BUILD_DIR}
fi
mkdir -p ${BUILD_DIR}

cd ${BUILD_DIR}
cmake -B . -S ${ROOT_PWD} \
    -DBUILD_TYPE=${BUILD_TYPE} \
    -DBUILD_STATIC=${BUILD_STATIC} \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -GNinja

ninja
ninja install
