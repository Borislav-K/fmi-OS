#!/bin/bash
#Да се напише shell скрипт, който получава единствен аргумент директория и отпечатва списък с всички файлове и директории в нея (без скритите).
#До името на всеки файл да седи размера му в байтове, а до името на всяка директория да седи броят на елементите
# в нея (общ брой на файловете и директориите, без скритите).

#a) Добавете параметър -a, който указва на скрипта да проверява и скритите файлове и директории.

#Пример:
#$ ./list.sh .
#asdf.txt (250 bytes)
#Documents (15 entries)
#empty (0 entries)
#junk (1 entry)
#karh-pishtov.txt (8995979 bytes)
#scripts (10 entries)

if [ $# -eq 0 ] || [ $# -gt 2 ]
then
	echo "Usage: $0 [-a]  <dir>"
	exit 2
fi
SHOW_HIDDEN="f"
if [ $# -eq 2 ]
then
	if [ "${1}" != "-a" ]
	then 
		echo "Usage: $0 [-a] <dir>"
		exit 2
	fi
	shift 1
    SHOW_HIDDEN="t"
fi

if [ ! -d $1 ]  || [ ! -r $1 ]
then
	echo "Error. $1 must be a readable directory"
	exit 1
fi

if [ "${SHOW_HIDDEN}" = "t" ]
	then
	FILES=$(find "${1}" -mindepth 1 -maxdepth 1 -type f 2>/dev/null -exec stat -c "%n (%s bytes)" {} \;)
	DIRECTORIES=$(find "${1}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | tr '\n' ' ') 
else
	FILES=$(find "${1}" -mindepth 1 -maxdepth 1 -type f 2>/dev/null -exec stat -c "%n (%s bytes)" {} \; | egrep -v "${1}/\.")
	DIRECTORIES=$(find "${1}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | egrep -v "${1}/\." | tr '\n' ' ')
fi
for DIR in ${DIRECTORIES}
do
	ENTRIES=$(find "${DIR}" -mindepth 1 -maxdepth 1 -type f -o -type d 2>/dev/null| sed -r "s/.*\/(.*)/\1/" | egrep -v "^\." | wc -l)
	echo "${DIR} (${ENTRIES} entries)"
done
echo "${FILES}"
