#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Usage: $0 <dir> (<int>)"
	exit 1
fi

if [ $# -gt 2 ]
then
	echo "Usage: $0 <dir> (<int>)"
	exit 1
fi

if [ ! -d $1 ]
then
	echo "Error: $1 must be a directory"
	exit 2
fi

if [ ! -r $1 ]
then
	echo "Error: $1 must be a readable directory"
	exit 2
fi


if [ $# -eq 1 ]
then
	find $1 -type l -exec file {} \; 2>/dev/null| egrep ".*: broken symbolic link" | cut -d: -f1
	exit 0
fi

#At this point we are sure that 2 parameters are given

find $1 -printf "%n %f\n" | awk -v N=$2 '$1>=N {print $2}' 2>/dev/null

