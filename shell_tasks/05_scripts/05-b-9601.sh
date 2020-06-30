#!/bin/bash
#Да се напише shell скрипт, който възстановява изтрити файлове, които имат запазено копие в BACKUP_DIR (от предната задача).
#При Възстановяването файловете да се декомпресират, а директориите да се декомпресират и разархивират.

#а) Да се дефинира параметър -l, който изрежда всички файлове, които могат да бъдат възстановени и датата на тяхното изтриване.

#б) Скриптът да приема 2 параметъра. Първият е името на файла, който да се възстанови, а вторията е директорията,
# в която файлът да бъде възстановен. Ако вторият аргумент липсва, файлът да се възстановява в сегашната директория, където скриптът се изпълнява.

#в) Когато има N > 1 запазени файла със същото име, да се изпише списък с N реда на потребителя и да се изиска той да
# въведе цяло число от 1 до N, за да избере кой файл да възстанови.
recover_file () {
	FILENAME=$(basename ${1})
	DSTDIR=${2}
	FILETYPE=$(echo "${FILENAME}" | rev | cut -d '.' -f 1 | rev)
	if [ "${FILETYPE}" = "gz" ]
	then
		SHORTNAME=$(echo "${FILENAME}" | sed -r 's/^(.*)_.*\.[t]?gz$/\1/')
		gunzip -c "${BACKUP_DIR}/${FILENAME}" > "${DSTDIR}/${SHORTNAME}"
	else
		tar -xzf "${BACKUP_DIR}/${FILENAME}" -C "${DSTDIR}"
	fi
	
}

list_matches () {
	echo "Choose a file to be recovered: " 
	FILENAME=${1}
	find "${BACKUP_DIR}" -mindepth 1 -name "${FILENAME}*" | sort | egrep -n "."
}
if [ $# -eq 0 ] || [ $# -gt 2 ]
then
	echo "Usage: ${0} -l OR ${0} <file> [<dir>]"
	exit 1
fi
#-l check
if [ "$1" = "-l" ]
then
	find "${BACKUP_DIR}" -mindepth 1 -printf "%f\n" | sed -r 's/(.*)_(.*)\..*/Name: \1, deleted at:\2/'
	exit
fi
#directory check
if [ $# -eq 1 ]
then
	DIR=$(pwd)
else
	DIR=$2
	if [ ! -d $2 ] || [ ! -w $2 ]
	then
		echo "Error. $2 must be a writable directory"
		exit 2
	fi
fi
#file check -checks whether the filename is a valid one in the backup directory
MATCHES_COUNT=$(find "${BACKUP_DIR}" -mindepth 1 -printf "%f\n" | sed -r 's/(.*)_(.*)\..*/\1/' | egrep "^${1}$" | wc -l)
if [ "${MATCHES_COUNT}" -eq 0 ]
then
	echo "Error. $1 is not a valid file"
	exit 3
fi

#Decompression:
if [ "${MATCHES_COUNT}" -eq 1 ]
then
	FILENAME=$(find "${BACKUP_DIR}" -mindepth 1 -name "${1}*" -printf "%f")
	recover_file "${FILENAME}" "${DIR}"
	exit
fi

#At this point, there are 2 or more matches
list_matches $1
read N
FILENAME=$(grep "^${N}" < <(list_matches $1) | cut -d ':' -f 2)
recover_file "${FILENAME}" "${DIR}"





