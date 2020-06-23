#!/bin/bash
#Да се отпечатат PID на всички процеси, които имат повече деца от родителския си процес.


PID_PPID=$(ps -eo pid=,ppid=)
while read CHILD PARENT
do
	PARENT_CHILDREN_COUNT=$(echo "${PID_PPID}" | egrep "${PARENT}$" | wc -l)
	CHILD_CHILDREN_COUNT=$(echo "${PID_PPID}"  | egrep "${CHILD}$" | wc -l)
	if [ ${CHILD_CHILDREN_COUNT} -gt ${PARENT_CHILDREN_COUNT} ]
	then
		echo "${CHILD}"
	fi
done < <(echo "${PID_PPID}")



