#!/usr/bin/env python2
# -*- coding=utf-8 -*-
import re
import sys


def is_number(num):
    """ 判断是否为数字 """
    try:
        int (num)
        return True
    except ValueError:
        return False
    return False

if __name__ == '__main__':
    if len (sys.argv) < 2:
        print ("挂载点不正确")
        sys.exit (-1)

    mountPath = sys.argv[1]     # 用户名

    allUUID = {}                # 所有设备信息
    allName = {}                # 所有名字设备
    fstabInfo = {}              # fstab 信息解析
    saveInfo = {}               # 要保存的

    # 获取所有设备
    with open ('all_device.txt', 'r') as fr:
        for line in fr:
            line = line.strip()
            arr = line.split(",")
            if len(arr) != 3 or arr[0] == "" or arr[1] == "" or arr[2] == "" or not is_number(arr[2][-1]):
                #print ("bad line:" + line)
                continue
            allUUID[arr[0]] = arr[1]
            allName[arr[2]] = arr[1]

    # 获取当前已自动挂载的设备
    with open ('/etc/fstab', 'r') as fr:
        for line in fr.readlines():
            line = line.strip()
            if (len(line) == 0) or ((len(line) > 0) and (line[0] == '#')):
                continue

            # 解析设备列表, 强行以\t分割
            line = re.sub(' ', '\t', line)
            line = re.sub('\t+', '\t', line)
            arr = line.split('\t')
            if len(arr) == 6:
                pths = arr[0].split('/')
                if len(pths) > 1:
                    print ('fstab 配置文件中文件系统表示使用了设备名而不是uuid')
                    sys.exit (-1)
                uuids = arr[0].split('=')
                if len(uuids) == 2:
                    fstabInfo[uuids[1]] = (arr[1], arr[2], arr[3], arr[4], arr[5])
                elif len(uuids) == 1:
                    fstabInfo[arr[0]] = (arr[1], arr[2], arr[3], arr[4], arr[5])
                else:
                    print ("fstab 文件格式解析出錯")
                    exit(-1)
            else:
                print ("fstab 配置文件的格式解析出錯")
                exit(-1)

    # 去重并追加
    for ik, iv in allUUID.items():
        if fstabInfo.has_key(ik):
            continue
        saveInfo[ik] = (mountPath + '/' + ik, iv, 'defaults', '0', '0')

    # 追加写入 fstab
    with open ('/etc/fstab', 'a') as fw:
        fw.write("\n")
        for ik, iv in saveInfo.items():
            fw.write("UUID=" + ik + "\t" + "\t".join(iv) + "\n")
            print ("磁盘UUID:" + ik + ' -- 将要自动挂载到:' + iv[0])

    print ("完成!")
    sys.exit(0)
