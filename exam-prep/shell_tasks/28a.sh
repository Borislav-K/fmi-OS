#!/bin/bash

if [ $# -ne 0 ]
then
	echo "Error. $0 is used without arguments"
	exit 1
fi

NUMBERS=$(mktemp)
MAX_ABS=0
while read LINE
do
	echo "${LINE}" | egrep -q "^[+-]?[0-9]+$"
	if [ $? -eq 0 ]
	then
		ABS=$(echo "${LINE}" | tr -d '+' | tr -d '-')
		if [ ${ABS} -ge ${MAX_ABS} ]
		then
			MAX_ABS=${ABS}
			echo "${LINE}" >> ${NUMBERS}
		fi
	fi
done

cat ${NUMBERS} | egrep "[+-]?${MAX_ABS}" | sort | uniq
rm -f ${NUMBERS}
