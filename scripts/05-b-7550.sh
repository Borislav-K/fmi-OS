#!/bin/bash
#Да се напише shell скрипт, който приема параметър - име на потребител. Скриптът да прекратява изпълненито на 
#всички текущо работещи процеси на дадения потребител, и да извежда колко са били те.

if [ $# -ne 1 ]
then
	echo "Usage: $0 <username>"
	exit 1
fi

who | awk '{print $1}' | grep "^${1}$"
if [ $? -ne 0 ]
then
	echo "$1 is currently not logged"
	exit 2
fi

COUNT=$(ps -eu "${1}" -o pid= | wc -l)
ps -eu "${1}" -o pid= | xargs kill
sleep 2
ps -eu "${1}" -o pid= | xargs kill -9
echo "${COUNT} processes killed"
