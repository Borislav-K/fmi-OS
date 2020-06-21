#!/bin/bash

if [ $# -eq 0 -o $# -gt 2 ]
then
	echo "Usage: $0 <dir> (<file>)"
	exit 1
fi

if [ ! -d $1 ]
then
	echo "Error. $1 must be a directory"
	exit 2
fi

if [ ! -r $1 ]
then
	echo "Error. $1 is not a readable directory"
	exit 2
fi

if [ $# -eq 2 ]
then
	if [ ! -f $2 ]
	then
		echo "Error. $2 is not a valid file"
		exit 3
	fi

	if [ ! -w $2 ]
	then
		echo "Error. You do not have write permissions for $2"
		exit 3
	fi
	LINKS=$(find $1 -type l 2>/dev/null -exec file {} \;)
	BROKEN_COUNT=$(echo "${LINKS}" | egrep "^(.*): broken" | wc -l)
	echo "${LINKS}" | egrep -v "^(.*): broken" | sed -r 's/(.*): symbolic link to (.*)/\1 -> \2/' >> $2
	echo "Broken symlinks: ${BROKEN_COUNT}" >> $2
fi

if [ $# -eq 1 ]
then
	LINKS=$(find $1 -type l 2>/dev/null -exec file {} \;) 	
	BROKEN_COUNT=$(echo "${LINKS}" | egrep "^(.*): broken" | wc -l)
	echo "${LINKS}" | egrep -v "^(.*): broken" | sed -r 's/(.*): symbolink link to (.*)/\1 -> \2/'
	echo "Broken symlinks: ${BROKEN_COUNT}"
fi
