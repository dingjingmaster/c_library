#!/bin/bash

work_dir=$(dirname $(realpath -- $0))

. "${work_dir}/common"

_msg_info_pure "====================================== 系统信息 ================================================"
_msg_info_pure "             arch: $(uname -m)"
_msg_info_pure "     make version: $(make --version | head -1)"
_msg_info_pure "      gcc version: $(${CC:-gcc} --version | head -1)"
_msg_info_pure "       ld version: $(ld -v | head -1)"
_msg_info_pure "           mkswap: $(mkswap -V 2>&1 | awk '{print "util-linux ", $NF}')"
_msg_info_pure "   kernel version: $(uname -r -v)"
_msg_info_pure "     command line: $(cat /proc/cmdline)"
_msg_info_pure "     load modules: $(cat /proc/modules | sed -e 's/ .*$//' | sed ':label;N;s/\n/ /g;b label' | sed 's/[ ][ ]*/ /g')"
_msg_info_pure ""
_msg_info_pure ""
_msg_info_pure "release info:"
_msg_info_pure " $(cat /etc/*release)"
_msg_info_pure ""
_msg_info_pure ""
_msg_info_pure "free report:"
_msg_info_pure " $(free -h)"
_msg_info_pure ""
_msg_info_pure ""
_msg_info_pure "cpu info:"
_msg_info_pure " $(cat /proc/cpuinfo)"
_msg_info_pure "================================================================================================"
