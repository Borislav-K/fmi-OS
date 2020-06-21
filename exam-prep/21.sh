#!/bin/bash

if [ "$(whoami)" != "root" ]
then
	echo "You are not root"
	exit 1
fi

COUNTS_USERS=$(ps -eo user= | sort | uniq -c | sed -r 's/[[:space:]]+(.*)/\1/')
PROCESSES=$(ps -eo user=,rss=)

while read COUNT USER
do
	RSS=$(echo "${PROCESSES}" | egrep "^${USER}" | awk '{sum+=$2} END {print sum}')
	if [ -z "${RSS}" ]
	then
		RSS="0"
	fi
	echo "User ${USER} : ${COUNT} processes, TOTAL RSS: ${RSS}"
done < <( echo "${COUNTS_USERS}")
