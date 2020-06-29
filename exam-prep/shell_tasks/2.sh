#!/bin/bash
#-1 --> Към всеки ред се добавя номер на ред
#-2 --> Премахват се данните за годината на издаване
#-3 --> Сортира се изходът по заглавие(лексикографски, възходящо)

if [ $# -ne 2 ]
then
	echo "Usage: $0 -1|-2|-3 <file>"
	exit 2
fi

if [ "$1" != "-1" ] && [ "$1" != "-2" ] && [ "$1" != "-3" ]
then
	echo "Usage: $0 -1|-2|-3 <file>"
	exit 2
fi

if [ ! -f $2 ] || [ ! -r $2 ]
then
	echo "Error. $2 is not a readable file"
	exit 1
fi

if [ "$1" = "-1" ]
then
	cat $2 | egrep -n "." | sed -r 's/:/\./'
	exit
fi

if [ "$1" = "-2" ]
then
	cat $2 | cut -d '-' -f 2-
	exit
fi

if [ "$1" = "-3" ]
then
	cat $2 | sed -r 's/(.*)[[:space:]]-[[:space:]](.*)-(.*)$/\1 - \2 - \3/'|sort -t '-' -k 2
fi
