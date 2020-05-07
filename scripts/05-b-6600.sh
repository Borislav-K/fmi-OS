#!/bin/bash
#Да се напише shell скрипт, който получава единствен аргумент директория и изтрива
#всички повтарящи се (по съдържание) файлове в дадената директория. Когато има няколко
#еднакви файла, да се остави само този, чието име е лексикографски преди имената
#на останалите дублирани файлове.

#Примери:
#$ ls .
#f1 f2 f3 asdf asdf2
# asdf и asdf2 са еднакви по съдържание, но f1, f2, f3 са уникални

#$ ./rmdup .
#$ ls .
#f1 f2 f3 asdf
#asdf2 е изтрит

if [ $# -ne 1 ]
then
	echo "Usage: $0 <directory>"
	exit 3
fi

if [ ! -d $1 ]
then
	echo "Error. $1 is not a directory"
	exit 2
fi

if [ ! -r $1 ]
then
	echo "Error. $1 is not a readable directory"
	exit 1
fi

KV_PAIRS=$(find "${1}" -mindepth 1 -maxdepth 1 -type f -print 2>/dev/null | xargs sha256sum 2>/dev/null | sort)
DUP_HASHES=$(echo "${KV_PAIRS}"| awk '{print $1}' | uniq -d)
for HASH in $DUP_HASHES
do
	echo "${KV_PAIRS}" | grep "${HASH}" | tail -n +2 |awk '{print $2}' |xargs rm
done
