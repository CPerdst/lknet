#!/usr/bin/env bash

function check_boost() {
    echo "正在检测Boost项目路径"
    TEMPDIR=$(mktemp -d)
    cat <<EOL > "${TEMPDIR}/CMakeLists.txt"
    find_package(Boost CONFIG REQUIRED)
EOL
    cmake -S "${TEMPDIR}/" -B "${TEMPDIR}/build" > /dev/null 2>&1
    if [[ $? != 0 ]];then
      rm -rf "${TEMPDIR}"
      return 0
    else
      rm -rf "${TEMPDIR}"
      return 1
    fi
}

function check_boostcondig_dir() {
  root_dir=$1
  if [[ -d ${root_dir} ]];then
    return 1
  else
    return 0
  fi
}

function check_submodule() {
  if [[ -d ${1}/CMakeLists.txt ]]; then
    return 0
  fi

  git submodule update --init --recursive --remote ${1}

  return 0
}

function select_build_type() {
    echo "（1）请选择编译模式:" >&2
    echo "1) Debug" >&2
    echo "2) Release" >&2

    read -r build_type_opt

    if [[ ${build_type_opt} == "1" ]];then
      build_type="Debug"
    elif [[ ${build_type_opt} == "2" ]];then
      build_type="Release"
    else
      echo "请输入正确选项" >&2
      exit
    fi

    echo ${build_type}
}

function select_crt_type() {
    echo "（2）请选择链接库输出模式" >&2
    echo "1) 静态" >&2
    echo "2) 动态" >&2

    read -r crt_type_opt

    if [[ ${crt_type_opt} == "1" ]];then
      crt_type="static"
    elif [[ ${crt_type_opt} == "2" ]];then
      crt_type="dynamic"
    else
      echo "请输入正确选项" >&2
      exit
    fi

    echo ${crt_type}
}

function select_example_test_build_if() {
    echo "（3）请选择是否编译输出使用样例与测试样例" >&2
    echo "1) 是" >&2
    echo "2) 否" >&2

    read opt

    if [[ ${opt} == "1" ]];then
      res="True"
    elif [[ ${opt} == "2" ]];then
      res="False"
    else
      echo "请输入正确选项" >&2
      exit
    fi

    echo ${res}
}

function select_build_tool() {
    echo "（3）请选择编译套件" >&2
    echo "1) GNU" >&2
    echo "2) Clang" >&2

    read build_tool_opt

    if [[ ${build_tool_opt} == "1" ]];then
      build_tool="gcc"
    elif [[ ${build_tool_opt} == "2" ]];then
      build_tool="clang"
    else
      echo "请输入正确选项" >&2
      exit
    fi

    echo ${build_tool}
}

function main_track() {
    boostconfig_root_dir=""
    # 首先检测是否能够找到Boost
    check_boost
    if [[ $? == 0 ]];then
      echo "BoostConfig.cmake绝对根路径:"
      read boostconfig_root_dir
    fi

    if [[ "${boostconfig_root_dir}" != "" ]];then
      check_boostcondig_dir ${boostconfig_root_dir}
      if [[ $? == 0 ]];then
        echo "路径指定出错，请重新输入"
      fi
    fi

    # 检查子模块
    check_submodule lklog

    # 选择编译模式
    build_type=$(select_build_type)

    # CRT输出模式
    crt_type=$(select_crt_type)

    # 是否编译样例
    example_test_build_if=$(select_example_test_build_if)

    # 选择编译套件（默认gcc、g++
    build_tool=$(select_build_tool)
    if [[ "${build_tool}" == "gcc" ]];then
      build_tool_c="gcc"
      build_tool_cxx="g++"
    elif [[ "${build_tool}" == "clang" ]];then
      build_tool_c="clang"
      build_tool_cxx="clang++"
    fi

    echo "command: "
    echo "cmake -DCMAKE_C_COMPILER=${build_tool_c} -DCMAKE_CXX_COMPILER=${build_tool_cxx}++" \
         "-DCMAKE_BUILD_TYPE=${build_type} -DCRT_BUILD_TYPE=${crt_type} -DEXAMPLE_BUILD=${example_test_build_if}" \
         "-DTEST_BUILD_TYPE=${example_test_build_if} -S . -B build-${build_type}-${build_tool}-${crt_type}"

    mkdir build-${build_type}-${build_tool}-${crt_type}
    pushd build-${build_type}-${build_tool}-${crt_type}

    cmake -DCMAKE_C_COMPILER=${build_tool} -DCMAKE_CXX_COMPILER=${build_tool}++ -DCMAKE_BUILD_TYPE=${build_type} -DCRT_BUILD_TYPE=${crt_type} -DEXAMPLE_BUILD=${example_test_build_if} -DTEST_BUILD_TYPE=${example_test_build_if} -S .. -B .

    cmake --build . -j6 --target all
}

main_track
