#!/bin/bash
#Да се напише shell скрипт, който чете от стандартния вход име на файл
#и символен низ, проверява дали низа се съдържа във файла и
#извежда на стандартния изход кода на завършване накомандата 
#с която сте проверили наличието на низа.

read  FILE STRING
echo "FILE: ${FILE}, STRING: ${STRING}"
if [ ! -e $FILE ]
then
	echo "Error. File $FILE does not exist"
	exit 1
fi

cat $FILE | grep "${STRING}" >/dev/null
STAT=$?
if [ $STAT -eq 0 ] 
then
	echo "String exists"
else 
	echo "String does not exist"
fi

echo "Exit status of grep: $STAT"

