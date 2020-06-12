#!/bin/bash

# Copyright 2018,2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#


SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

CLANG_FORMAT_FIND="find"
CMAKE_FORMAT_PREFIX="echo "
CMAKE_FORMAT_FIND="${CMAKE_FORMAT_PREFIX} find"
CMAKE_FORMAT_GIT="${CMAKE_FORMAT_PREFIX} git"
CMAKE_FORMAT_PYTHON="${CMAKE_FORMAT_PREFIX} python"

#echo SCRIPTPATH=${SCRIPTPATH}
cd ${SCRIPTPATH}/..

for d in sss doc/input hostlib/useCase akm demos/se05x demos/a71ch mal/lib
do
    ${CLANG_FORMAT_FIND} $d -type f -name "*.h" -print -exec clang-format -i "{}" ";"
    ${CLANG_FORMAT_FIND} $d -type f -name "*.c" -print -exec clang-format -i "{}" ";"
    ${CLANG_FORMAT_FIND} $d -type f -name "*.cpp" -print -exec clang-format -i "{}" ";"
    ${CLANG_FORMAT_FIND} $d -type f -name "*.hpp" -print -exec clang-format -i "{}" ";"
    ${CLANG_FORMAT_FIND} $d -type f -name "*.c.txt" -print -exec clang-format -i "{}" ";"
done


for d in demos sss tst doc/input hostlib hostlib/hostLib/se05x hostlib/hostLib/se05x_03_xx_xx akm scripts
do
    ${CMAKE_FORMAT_FIND} $d -type f -name "CM*.txt" -print -exec cmake-format -i "{}" -c ${SCRIPTPATH}/cmake-format.json ";"
    ${CMAKE_FORMAT_FIND} $d -type f -name "*.cmake" -print -exec cmake-format -i "{}" -c ${SCRIPTPATH}/cmake-format.json ";"
done

cd ${SCRIPTPATH}

${CMAKE_FORMAT_GIT} checkout -- CodeCoverage.cmake
${CMAKE_FORMAT_GIT} checkout -- ToolchainFile_imx6.cmake
${CMAKE_FORMAT_GIT} checkout -- cmake_options.cmake

${CMAKE_FORMAT_PYTHON} cmake_options.py
