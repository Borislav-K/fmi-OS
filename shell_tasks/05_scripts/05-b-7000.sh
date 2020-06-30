#!/bin/bash
#Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове.
# Скриптът да прочита от стандартния вход символен низ и за всеки от зададените файлове извежда по подходящ начин
# на стандартния изход броя на редовете, които съдържат низа.

#NB! Низът може да съдържа интервал.

if [ $# -eq 0 ]
then
	echo "Error. At least 1 file name must be given "
	exit 2
fi

for FILE in "$@"
do
	if [ ! -f $FILE ] || [ ! -r $FILE ]
	then
		echo "Error. $FILE is not a readable file"
		exit 1
	fi
done

read STRING
echo "String is: ${STRING}"
for FILE in "$@"
do
	COUNT=$(cat "${FILE}" | grep "${STRING}" | wc -l)
	echo "$COUNT lines for file $FILE"
done
