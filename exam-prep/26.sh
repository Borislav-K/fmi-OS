#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <text_file> <dir>"
	exit 1
fi

if [ ! -f $1 ] || [ ! -r $1 ]
then
	echo "Error. $1 is not a readable file"
	exit 2
fi
file $1 | grep -q "ASCII"
if [ $? -ne 0 ]
then
	echo "Error. $1 is not an ASCII text file"
	exit 2
fi

if [ ! -d $2 ] || ! [ -w $2 ]
then
	echo "Error. $2 is not a writable directory"
	exit 3
fi

FILES_COUNT=$(find $2 -mindepth 1 | wc -l)
if [ ${FILES_COUNT} -ne 0 ]
then
	echo "Error. $2 is not an empty directory"
	exit 3
fi

ID=0
touch "${2}/dict.txt"

FULLNAMES=$(mktemp)
while read NAME1 NAME2
do
	echo "${NAME1} ${NAME2}" >> ${FULLNAMES}
done < <( cat $1 | sed -r 's/\(.*\)//' | cut -d: -f1 | tr -s ' ')


while read FULLNAME
do
	echo "${FULLNAME};${ID}" >> "${2}/dict.txt"
	ID=$((${ID}+1))
done < <(cat ${FULLNAMES} | sort | uniq)


rm -f ${FULLNAMES} 
