#/bin/bash
#Да се напише shell скрипт, който чете от стандартния вход
#имената на 3 файла, обединява редовете на първите два (man paste), 
#подрежда ги по азбучен ред и резултата записва в третия файл.

if [ $# -ne 3 ]
then
	echo "Error. Required 3 arguments"
fi

if [ ! -e $1 ]
then
	echo "File $1 does not exist"
	exit 1
fi

if [ ! -e $2 ]
then
	echo "File $2 does not exist"
	exit 2
fi

if [ ! -e $3 ]
then
	echo "File $3 does not exist"
	exit 3
fi

if [ ! -r $1 ]
then
	echo "File $1 is not readable"
	exit 4
fi

if [ ! -r $2 ]
then
	echo "File $2 is not readable"
	exit 5
fi

if [ ! -w $3 ]
then
	echo "File $3 is not readable"
	exit 6
fi

paste $1 $2 | sort > $3
