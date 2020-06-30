#!/bin/bash
#Напишете shell script, който получава като единствен аргумент име на потребител и за всеки негов процес изписва съобщение 
#за съотношението на RSS към VSZ. Съобщенията да са сортирани, като процесите с най-много заета виртуална памет са най-отгоре.

if [ $# -ne 1 ]
then
	echo "Usage: $0 <username>"
	exit 2
fi

who | awk '{print $1}' | egrep -q "^${1}$"
if [ $? -ne 0 ]
then
	echo "Error. User $1 not logged"
	exit 1
fi

while read PID RSS VSZ
do
	PROPORTION="inf"
	if [ ${VSZ} -ne 0 ]
	then
		PROPORTION=$(echo "scale =2; ${RSS}/${VSZ}" | bc)
		#PROPORTION=$(echo "${RSS} ${VSZ}" | awk '{print $1/$2}')
	fi
	echo "PID: ${PID} RSS/VSZ: ${PROPORTION}"
done < <(ps -eu "${1}" -o pid,rss,vsz | tail -n +2 | sort -nrk 3)

