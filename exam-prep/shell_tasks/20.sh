#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Error. Usage: $0 <SRC> <DST> <STR>"
	exit 1
fi

if [ "$(whoami)" != "root" ]
then
	echo "Error. You must be root"
	exit 2
fi


if [ ! -d $1 ] || [ ! -r $1 ]
then
	echo "$1 must be a readable directory"
	exit 3
fi

if [ ! -d $2 ] || [ ! -w $2 ]
then
	echo "$2 must be a writable directory"
	exit 3
fi

if [ $(find $2 -mindepth 1 | wc -l) -ne 0 ]
then
	echo "Error. $2 is not an empty directory"
	exit 3
fi

FILES_TO_MOVE=$(find $1 -type f -name "*${3}*")
while read FILE
do
	mv ${FILE} $(echo "${FILE}" | sed -r "s/^.*\/(.*)$/${2}\/\1/")
done < <(echo "${FILES_TO_MOVE}")
