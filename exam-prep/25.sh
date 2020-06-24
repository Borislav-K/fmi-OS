#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <LOGDIR>"
	exit 1
fi

if [ ! -d $1 ] || [ ! -r $1 ]
then
	echo "Error: $1 is not a readable directory"
	exit 2
fi

FRIENDS=$(find $1 -mindepth 3 -maxdepth 3 -type d -printf "%f\n" | sort | uniq)
RECORDS=$(mktemp)
for FRIEND in ${FRIENDS}
do
	TOTAL_LINES=0
	ALL_DIRS=$(find $1 -mindepth 3 -maxdepth 3 -type d -name "${FRIEND}" -print)
	for DIR in ${ALL_DIRS}
	do
	LINES=$(find ${DIR} -type f -exec wc -l {} \; | awk '{sum+=$1} END {print sum}')
	if [ ! -z ${LINES} ]
	then
		TOTAL_LINES=$((${TOTAL_LINES}+${LINES}))
	fi
	done
	echo "${TOTAL_LINES} ${FRIEND}"
done

cat ${RECORDS} | sort -t ' ' -nk1 | tail -10 
