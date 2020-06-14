#!/bin/bash
#Напишете shell script, който по подаден като аргумент файл с map
#между <nickname> -> <реален username> и nickname, ще ви улесни 
#да изпращате съобщения на хората.
#Пример за файл указател:
#tinko s61966
#minko s881234
#ginko s62000
#dinko s77777
#Примерно извикване на програмата:
#./send_message myAddressBook dinko
#И вече може да изпращате съобщения на човека с username s77777

if [ $# -ne 2 ]
then
	echo "Error. Example usage: $0 <map> <nickname>"
	exit 1
fi

if [ ! -r $1 ]
then
	echo "Error. File $1 does not exist or is not readable"
	exit 1
fi

USERNAME=$(cat $1 | egrep "^${2}" | awk '{print $2}')

if [ $? -eq 1 ]
then
	echo "Error. Nickname ${2} not found"
	exit 2
fi

read -p "Enter the message you want to send to ${2}: " MSG

echo $MSG | write $USERNAME


