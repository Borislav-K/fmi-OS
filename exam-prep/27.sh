#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <csv1> <csv2>"
	exit 1
fi


if [ ! -f $1 ] || [ ! -r $1 ]
then
	echo "Error. $1 must be a readable file"
	exit 2
fi

if [ -e $2 ]
then
	if [ ! -f $2 -o ! -w $2 ]
	then
		echo "Error. $2 already exists and is not a writable, ordinary file"
		exit 3
	else
		echo -n "" > $2 #Truncate the file
	fi
fi

if [ ! -e $2 ]
then
	touch $2
fi

while read LINE
do
	LINE_ID=$(echo ${LINE} | cut -d, -f1)
	LINE_CONTENT="$(echo ${LINE} | cut -d, -f2-)"
	cat $2 | egrep -q "^[0-9]+,${LINE_CONTENT}$"	
	if [ $? -eq 0 ]
	then
		#skip this line - already added
		continue
	fi
	MATCHING_IDS=$(egrep "^[0-9]+,${LINE_CONTENT}$" $1 | cut -d, -f1)
	LOWEST_ID=$(echo "${MATCHING_IDS}" | sort -n | head -1)
	echo "${LOWEST_ID},${LINE_CONTENT}" >> $2
	
done < $1
