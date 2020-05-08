#!/bin/bash
#Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове или директории. 
#Скриптът да извежда за всеки аргумент подходящо съобщение:
	#- дали е файл, който може да прочетем
	#- ако е директория - имената на файловете в нея, които имат размер, по-малък от броя на файловете в директорията.

if [ $# -eq 0 ]
then
	echo "$0 requires at least 1 argument"
	exit 2
fi

for ARG in "$@"
do
	if [ ! -e ${ARG} ]
	then
		echo "Error: file ${ARG} does not exist"
		exit 1
	fi
done

for ARG in "$@"
do
	if [ -f ${ARG} ]
	then
		if [ -r $ARG ]
		then
			echo "$(basename ${ARG}) is a readable file"
		else
			echo "$(basename ${ARG}) is not a readable file"
		fi
	elif [ -d ${ARG} ]
	then
		FCOUNT=$(find "${ARG}" -mindepth 1 -maxdepth 1 -type f 2>/dev/null | wc -l)
		echo -n "Directory ${ARG} has ${FCOUNT} file(s) "
		FILES=$(find "${ARG}" -mindepth 1 -maxdepth 1 -type f -size -"${FCOUNT}"c -exec basename {} \; 2>/dev/null | tr '\n' ' ')
		echo "and these are the files that have less size than ${FCOUNT} bytes: ${FILES}"
	else
		echo "${ARG} is neither a regular file, nor a directory"
	fi
done
