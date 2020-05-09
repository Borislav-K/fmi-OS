#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Usage: $0 <uint1> <uint2>"
	exit 2
fi

echo -en "${1}\n${2}" | grep -v "[0-9]"
if [ $? -eq 0 ]
then
	echo "Usage: $0 <uint1> <uint2>"
	exit 2
fi

mkdir a b c 2>/dev/null
while read FILE
do
	LINES=$(echo "${FILE}" | awk '{print $1}')
	NAME=$(echo "${FILE}" | awk '{print $2}')
	if [ $LINES -lt $1 ]
	then
		mv ${NAME} a
	elif [ $LINES -gt $2 ]
	then
		mv ${NAME} c
	else
		mv ${NAME} b
	fi
done < <(find . -mindepth 1 -type f -exec wc -l {} \;)
