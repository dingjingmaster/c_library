#!/bin/bash

######################### 使用前说明 ########################
# 使用前需要在 /etc/crontab 里配置PATH路径，使常用命令可用  #
# SHELL=/bin/bash                                           #
# PATH=/bin:/sbin:/usr/bin:/usr/sbin/                       #
#############################################################

set -e -u

workdir="$(dirname $(realpath -- $0))/reboot/"

rootPWD="qwe123"
logFile="${workdir}/run.log"
dmesg_logfile="${workdir}/Dmesg.log"
syslog_file="${workdir}/Syslog.log"

if (( EUID != 0 )); then
    echo "必须以root权限运行..."
    exit 1
fi

mkdir -p "${workdir}"
rm -rf "${workdir}/run.log" "${workdir}/Dmesg*" "${workdir}/Sys*"

inputSTR[0]="#!/bin/bash"
inputSTR[1]="source ${workdir}/times.left"
inputSTR[2]="rm -rf ${workdir}/times.left"
inputSTR[3]="if [ \${timesLeft} -gt '0' ];then"
inputSTR[4]="	echo \"还剩余\$((timesLeft-1))次重启。\" >> ${logFile}"
inputSTR[5]="   echo ${rootPWD} | sudo -S cat /var/log/syslog > ${syslog_file}-\${timesLeft}"
inputSTR[6]="   echo ${rootPWD} | sudo -S dmesg > ${dmesg_logfile}-\${timesLeft}"
inputSTR[7]="   echo ${rootPWD} | sudo -S cat /dev/null > /var/log/syslog"
inputSTR[8]="	date >> ${logFile}"
inputSTR[9]="	echo >> ${logFile}"
inputSTR[10]="	echo timesLeft=\$((timesLeft-1)) > ${workdir}/times.left"
inputSTR[11]="	reboot"
inputSTR[12]="else"
inputSTR[13]="	sed -i '/.\/rbt.sh/d' /etc/crontab"
inputSTR[14]="	rm -rf ${workdir}/rbt.sh"
inputSTR[15]="	chown ${USER}:${USER} ${logFile}"
inputSTR[16]="fi"

echo -n -e "请输入自动重启的次数:\t"
read timesLeft
echo "timesLeft=${timesLeft}" > ${workdir}/times.left

echo -n -e "请输入重启间隔的时间（分钟）:\t"
read interval

echo -e "Checking necessary files for auto rebooting ..."

if [ ! -f "${workdir}/rbt.sh" ];then
    rm -rf "${workdir}/rbt.sh"
	for ((i=0; i<${#inputSTR[@]}; i++))
	do
		echo ${inputSTR[$i]} >> ${workdir}/rbt.sh
	done
	echo ${rootPWD} | sudo -S chown root:root ${workdir}/rbt.sh
	echo ${rootPWD} | sudo -S chmod a+x ${workdir}/rbt.sh
fi
echo -e "\e[1;32m[ REBOOT SCRIPT OK]\e[0m"

oldIFS=$IFS
IFS=$'\n'
cfgOK=$(cat /etc/crontab | grep "rbt.sh" | wc -l)
if [ 0 -eq ${cfgOK} ];then
	echo "*/${interval} * * * * root cd ${workdir} && bash ./rbt.sh" >> /etc/crontab
fi
IFS=$oldIFS

echo -e "\e[1;32m[CRONTAB CONFIG OK]\e[0m"

echo -n "请稍候，系统即将重启中……"
read
