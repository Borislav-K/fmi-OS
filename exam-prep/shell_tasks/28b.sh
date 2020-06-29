#!/bin/bash

if [ $# -ne 0 ]
then
	echo "Error. $0 is used without CL arguments"
	exit 1
fi


NUMBERS=$(mktemp)
MAX_DIGIT_SUM=0
while read LINE
do
	echo "${LINE}" | egrep -q "^[+-]?[0-9]+$"
	if [ $? -eq 0 ]
	then
		DIGIT_SUM=$(echo "${LINE}" | tr -d '+' | tr -d '-' | sed -r 's/(.)/\1\n/g' | awk '{sum+=$1} END {print sum}')
		if [ ${DIGIT_SUM} -ge ${MAX_DIGIT_SUM} ]
		then
			MAX_DIGIT_SUM=${DIGIT_SUM}
			echo "${LINE}:${DIGIT_SUM}" >> ${NUMBERS}
		fi
	fi
done
cat ${NUMBERS} | awk -v SUM=${MAX_DIGIT_SUM} -F ':' '$2==SUM {print $1}'

rm -f ${NUMBERS}
