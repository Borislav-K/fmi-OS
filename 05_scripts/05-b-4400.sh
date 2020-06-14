#!/bin/bash
#Напишете shell script, който да приема параметър име на директория, от която
#взимаме файлове, и опционално експлицитно име на директория, в която ще
#копираме файлове. Скриптът да копира файловете със съдържание, променено
#преди по-малко от 45 мин, от първата директория във втората директория.
#Ако втората директория не е подадена по име, нека да получи такова от 
#днешната дата във формат, който ви е удобен. При желание
#новосъздадената директория да се архивира.

if [ $# -eq 0 ] || [ $# -gt 2 ]
then
	echo "Usage: $0 dir1 [dir2]"
	exit 1
fi

if [ ! -r $1 ] || [ ! -d $1 ]
then
	echo "Error: $1 must be a readable directory"
	exit 2
fi

DEST_DIR=$2
if [ $# -eq 2 ]
then
	if [ ! -w $2 ] || [ ! -d $2 ]
	then
		echo "Error: $2 must be a writable directory"
		exit 3
	fi
else 
	DEST_DIR="dest-dir-$(date +'%Y-%m-%d-%H:%m')"
	mkdir ${DEST_DIR}
fi

find ${1} -mindepth 1 -maxdepth 1 -cmin -45 -exec cp {} $DEST_DIR \;
