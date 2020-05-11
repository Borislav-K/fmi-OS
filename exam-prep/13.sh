#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <dir>"
	exit 1
fi

if [ ! -d $1 ]
then
	echo "$1 is not a directory"
	exit 2
fi

if [ ! -r $1 ]
then
	echo "$1 is not a readable directory"
	exit 3
fi
find $1 -mindepth 1 -type l 2>/dev/null | xargs file | grep "broken symbolic link" | awk -F ':' '{print $1}'
