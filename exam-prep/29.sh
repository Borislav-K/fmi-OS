#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Error. Usage: $0 [-n N] log1...logN"
	exit 1
fi

N=10 #Default value
if [ "$1" == "-n" ]
then
	echo "${2}" | egrep -q "^[0-9]$"
	if [ $? -ne 0 ]
	then
		echo "Error: $2 must be a number"
		exit 2
	fi
	N=$2
	shift 2
fi

OUTPUT=$(mktemp)
while [ $# -ne 0 ]
do
	if [ -f $1 ] && [ -r $1 ]
	then
		IDF=$(echo "${1}" | sed -r 's/(.*).log/\1/')
		cat $1 | tail -n ${N} | sed -r "s/^([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}) (.*)$/\1 ${IDF} \2/" >> ${OUTPUT}
	fi
	shift 1
done

cat ${OUTPUT} | sort
rm -f ${OUTPUT}
