#!/bin/bash
#Да се напише shell скрипт, който валидира дали дадено цяло число попада в 
#целочислен интервал. Скриптът приема 3 аргумента: числото, което трябва да се провери; лява граница на интервала; дясна граница на интервала. Скриптът
#да връща exit status:
#- 3, когато поне един от трите аргумента не е цяло число
#- 2, когато границите на интервала са обърнати
#- 1, когато числото не попада в интервала
#- 0, когато числото попада в интервал
if [ $# -ne 3 ]
then
	echo "Usage: $0 <num> <left_border> <right_border>"
	exit 4
fi
echo -e "${1}\n${2}\n${3}" | egrep -vq "^[+-]?[0-9]+$"
if  [ $? -eq 0 ]
then
	exit 3
fi

if [ $2 -gt $3 ]
then
	exit 2
fi

if [ $1 -gt $1 ] || [ $1 -lt $2 ]
then
	exit 1
fi
exit 0
