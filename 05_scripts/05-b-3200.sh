#!/bin/bash
#Да се напише shell скрипт, който приканва потребителя да въведе пълното име на#директория и извежда на стандартния изход подходящо съобщение
#за броя на всички файлове и всички директории в нея.

read -p "Enter a directory name: " DIR

if [ -d $DIR ] 
then 
	NFILES=$(find $DIR -mindepth 1 -maxdepth 1 -type f 2>/dev/null | wc -l)
	NDIRS=$(find $DIR -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)
	echo "Files count: $NFILES"
	echo "Directories count: $NDIRS"
else
	echo "Directory doesn't exist"
	exit 1
fi
