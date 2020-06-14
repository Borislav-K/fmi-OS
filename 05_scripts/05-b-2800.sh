#!/bin/bash
#Да се напише shell скрипт, който приема точно един параметър и проверява
#дали подаденият му параметър се състои само от букви и цифри.
if [ $# -ne 1 ]
then 
	echo "Usage: <cmd> <arg1>"
	exit 1
fi
echo "${1}" | egrep -vq "^[[:alnum:]]+$"
if [ $? -eq 0 ]
then
	echo "Not an Alphanumberic variable"
else 
	echo "Consists of numbers and letters only"
fi
exit
