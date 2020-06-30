#!/bin/bash
#Да се напише shell скрипт, който приема файл и директория. Скриптът проверява в подадената директория и нейните под-директории 
#дали съществува копие на подадения файл и отпечатва имената на намерените копия, ако съществуват такива.

#NB! Под 'копие' разбираме файл със същото съдържание.

if [ $# -ne 2 ]
then
	echo "Usage: $0 <fname> <dirname>"
	exit 1
fi

FILE=$1
DIR=$2

if [ ! -f $FILE ]
then
	echo "Error. $FILE is not a valid file"
	exit 2
fi

if [ ! -r $FILE ]
then
	echo "Error. $FILE is not readable"
	exit 3
fi

if [ ! -d $DIR ]
then
	echo "Error. $DIR is not a directory"
	exit 4
fi

if [ ! -r $DIR ]
then
	echo "Error. $DIR is not a readable directory"
	exit 5
fi

FILESIZE=$(stat "${FILE}" -c "%s")
FILEHASH=$(sha256sum "${FILE}" | awk '{print $1}')

CANDIDATES=$(find "${DIR}" -type f -size "${FILESIZE}c" -printf "%p\n" 2>/dev/null)
for CAN in $CANDIDATES
do
	CANDIDATE_HASH=$(sha256sum $CAN 2>/dev/null| awk '{print $1}')
	if [ "${CANDIDATE_HASH}" = "${FILEHASH}" ]
	then echo "${CAN}"
	fi
done



