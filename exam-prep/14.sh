#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <integer>"
	exit 2
fi

if ! egrep -q "^[+-]?[0-9]+$" <(echo $1)
then
	echo "$1 is not an integer"
	exit 1
fi

if [ ! "$(whoami)" = "root" ]
then
	exit 
fi

USERS=$(ps -e -o user= | sort | uniq)
for USER in $USERS
do
	TOTAL_RSS=$(ps -u "${USER}" -o pid=,rss= | awk '{sum+=$2} END {print sum}')
	echo "Total RSS for ${USER}: ${TOTAL_RSS}"
	if [ ${TOTAL_RSS} -gt ${1} ]
	then
		PID=$(ps -u "${USER}" -o pid=,rss= | sort -nrk 2 | head -n 1 | awk '{print $1}')
		kill $PID
		sleep 1
		kill -9 $PID
	fi		
done
