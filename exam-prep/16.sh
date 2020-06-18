#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Usage: $0 <file> <str1> <str2>"
	exit 1
fi

if [ ! -f $1 ]
then
	echo "Error. $1 is not a valid file"
	exit 1
fi

FIRST_LINE=$(cat "${1}" | egrep "^${2}=")
SECOND_LINE=$(cat "${1}" | egrep "^${3}=")

if [ -z "$FIRST_LINE" ] 
then
	echo "Error. $2 key not found in $1"
	exit 2
fi

if [ -z "$SECOND_LINE" ]
then
	echo "No $3 key found in $1"
	exit 0
fi

FIRST_VALUE=$(echo "${FIRST_LINE}" | cut -d '=' -f2)
SECOND_VALUE=$(echo "${SECOND_LINE}" | cut -d '=' -f2)
NEW_SECOND=""
FORBIDDEN=${FIRST_VALUE}
for i in ${SECOND_VALUE}
do
	echo "${FORBIDDEN}" | sed -r 's/[[:space:]]/\n/g'| egrep -q "^${i}$"
	if [ $? -eq 1 ]
	then
		NEW_SECOND=$(echo "${NEW_SECOND} ${i}")
		FORBIDDEN=$(echo "${FORBIDDEN} ${i}")
	fi
done

#Remove trailing whitespace(if not empty)
NEW_SECOND=$(echo "${NEW_SECOND}" | sed -r 's/ (.*)/\1/')

sed -ri "s/(${3}=).*/\1${NEW_SECOND}/" $1
