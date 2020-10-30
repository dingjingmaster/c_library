#!/bin/bash

rm -rf run.log Dmesg* Sys*

rootPWD="123123"
logFile=`pwd`/run.log
dmesg_logfile=`pwd`/Dmesg.log
syslog_file=`pwd`/Syslog.log

inputSTR[0]="#!/bin/bash"
inputSTR[1]="source /opt/times.left"
inputSTR[2]="rm -rf /opt/times.left"
inputSTR[3]="if [ \${timesLeft} -gt '0' ];then"
inputSTR[4]="	echo \"还剩余\$((timesLeft-1))次重启。\" >> ${logFile}"
inputSTR[5]="   echo ${rootPWD} | sudo -S cat /var/log/syslog > ${dmesg_logfile}-\${timesLeft}"
inputSTR[6]="   echo ${rootPWD} | sudo -S dmesg > ${syslog_file}-\${timesLeft}"
inputSTR[7]="   echo ${rootPWD} | sudo -S cat /dev/null > /var/log/syslog"
inputSTR[8]="	date >> ${logFile}"
inputSTR[9]="	echo >> ${logFile}"
inputSTR[10]="	echo "timesLeft=\$\(\(timesLeft-1\)\)" > /opt/times.left"
inputSTR[11]="	reboot"
inputSTR[12]="else"
inputSTR[13]="	sed -i '/.\/rbt.sh/d' /etc/crontab"
inputSTR[14]="	rm -rf /opt/rbt.sh"
inputSTR[15]="	chown ${USER}:${USER} ${logFile}"
inputSTR[16]="fi"

echo -n -e "请输入自动重启的次数:\t"
read timesLeft
echo "timesLeft=${timesLeft}" > `pwd`/times.left
echo ${rootPWD} | sudo -S mv `pwd`/times.left /opt/times.left &> /dev/null

echo -n -e "请输入重启间隔的时间（分钟）:\t"
read interval

echo -e "Checking necessary files for auto rebooting ..."

if [ ! -f /opt/rbt.sh ];then
	for ((i=0; i<${#inputSTR[@]}; i++))
	do
		echo ${inputSTR[$i]} >> `pwd`/rbt.sh
	done
	echo ${rootPWD} | sudo -S mv `pwd`/rbt.sh /opt/rbt.sh
	echo ${rootPWD} | sudo -S chown root:root /opt/rbt.sh
	echo ${rootPWD} | sudo -S chmod a+x /opt/rbt.sh
fi
echo -e "\e[1;32m[ REBOOT SCRIPT OK]\e[0m"

oldIFS=$IFS
IFS=$'\n'
cfgOK=$(cat /etc/crontab | grep "./rbt.sh")
if [ ! ${cfgOK} ];then
	echo ${rootPWD} | sudo -S sed -i "\$a\*\/${interval:=2} * * * * root cd \/opt && .\/rbt.sh" /etc/crontab
fi
IFS=$oldIFS
echo -e "\e[1;32m[CRONTAB CONFIG OK]\e[0m"

echo -n "请稍候，系统即将重启中……"
read
