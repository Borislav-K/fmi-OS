#!/bin/bash
#Да се напише shell скрипт, който форматира големи числа, за да са по-лесни за четене.
#Като пръв аргумент на скрипта се подава цяло число.
#Като втори незадължителен аргумент се подава разделител. По подразбиране цифрите се разделят с празен интервал.

#Примери:
#$ ./nicenumber.sh 1889734853
#1 889 734 853

#$ ./nicenumber.sh 7632223 ,
#7,632,223


if [ $# -eq 0 ] || [ $# -gt 2 ]
then
	echo "Usage: $0 <int> [<delim>]"
	exit 2
fi

echo "${1}" | egrep -vq "^[+-]?[0-9]+$"
if [ $? -eq 0 ] 
then
	echo "Error. First Argument must be an integer"
	exit 1
fi

DELIM=" "
if [ $# -eq 2 ]
then
	DELIM="${2}"
fi

NUMBER=$1
SIGN=""
echo $1 | egrep -q "[+-]"
if [ $? -eq 0 ]
then
	NUMBER=$( echo $1 | cut -c 2-)
	SIGN=$(echo $1 | egrep -o "[+-]")
fi

NUMBER=$(echo $NUMBER | rev | sed -r "s/([0-9]{3})/\1${DELIM}/g" | sed -r 's/[^0-9]{1}$//' | rev)
echo "${SIGN}${NUMBER}"
