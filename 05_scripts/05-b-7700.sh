#!/bin/bash
#Да се напише shell скрипт, който приема два параметъра - име на директория и число. Скриптът да извежда сумата от размерите
# на файловете в директорията, които имат размер, по-голям от подаденото число.

if [ $# -ne 2 ]
then
	echo "Usage $0 <dir> <int>"
	exit 2
fi

if [ ! -d $1 ] || [ ! -r $1 ]
then
	echo "Error. $1 must be a readable directory"
	exit 1
fi

echo "${2}" | egrep -vq "^[+]?[0-9]+$"
if [ $? -eq 0 ]
then
	echo "Error. $2 must be a positive integer"
	exit 1
fi

find ${1} -mindepth 1 -maxdepth 1 -type f -size "+${2}c" -exec stat -c "%s\n" {} \; | awk '{sum+=$1} END {print sum}'

