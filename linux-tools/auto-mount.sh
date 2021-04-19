#!/bin/bash

if [ $# -lt 1 ]
then
    echo -e "\033[32m 请输入要挂载的目录! \033[0m"
    exit 1
fi

mountPath="$1"

if [ $UID -ne 0 ];then
    echo -e "\033[32m 非root用户无法执行操作! \033[0m"
    exit 1
fi

if [ -d "${mountPath}" ]
then
    lsblk -npfo "UUID,FSTYPE,NAME" | awk '{print $1","$2","$3}' > all_device.txt

    python auto-mount.py ${mountPath}
else
    echo -e "\033[32m 要求的挂载目录不存在! \033[0m"
fi



