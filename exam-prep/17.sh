#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <uname>"
	exit 1
fi

users | grep -q ${1}
if [ $? -ne 0 ]
then
	echo "Error. User $1 not logged in"
	exit 2
fi

if [ "$(whoami)" != "root" ]
then
	echo "Error. You are not root"
	exit 3
fi

#a)
while read USER
do
	if [ "${USER}" = "${1}" ]
	then
		break
	fi
	echo "${USER}"
done < <(ps -eo user= | sort | uniq -c |sort -rnk1 | awk '{print $2}')

#b)
USERS=$(users |sed -r 's/(.*) /\1\n/g' | sort | uniq)
AVG_TIME=""
for USER in ${USERS}
do
	COUNT=$(ps -u $USER | tail -n +2 |wc -l)
	TOTAL_TIME=$(ps -u $USER -o times= | awk '{sum+=$1} END {print sum}')
	if [ "${TOTAL_TIME}" -eq 0 ]
	then
		AVG_TIME="0"
	else
		AVG_TIME=$((${COUNT}/${TOTAL_TIME}))
	fi
done



#c)
UPPER_LIMIT="$((${AVG_TIME}*2))"
NAUGHTY_PROCESSES=$(ps -u $1 -o pid=,times= | awk -v U=${UPPER_LIMIT} \
'$2>U {print $1}')
for PID in ${NAUGHTY_PROCESSES}
do
	echo "Sending SIGTERM to ${PID}"
	kill ${PID}
	sleep 1
	ps -p ${PID} | tail -n +2 | egrep -q ".*"
	if [ $? -eq 0 ]
	then
		echo "Sending SIGKILL to ${PID}"
	kill -9 ${PID}
	fi
done

