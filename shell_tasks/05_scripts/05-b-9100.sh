#!/bin/bash
#Опишете поредица от команди или напишете shell скрипт, които/който при известни две директории SOURCE и DESTINATION:
#- намира уникалните "разширения" на всички файлове, намиращи се някъде под SOURCE.
# (За простота приемаме, че в имената на файловете може да се среща символът точка '.' максимум веднъж.)
#- за всяко "разширение" създава по една поддиректория на DESTINATION със същото име
#- pазпределя спрямо "разширението" всички файлове от SOURCE в съответните поддиректории в DESTINATION

if [ $# -ne 2 ]
then
	echo "Usage: $0 <src> <dest>"
	exit 3
fi

if [ ! -d $1 ] || [ ! -r $1 ]
then
	echo "Error. $1 must be a readable directory"
	exit 2
fi

if [ ! -d $2 ] || [ ! -w $2 ]
then
	echo "Error. $2 must be a writable directory"
	exit 1
fi
while read EXT
do
	DIRNAME="${2}/${EXT}"
	mkdir "${DIRNAME}" 2>/dev/null
	find $1 -mindepth 1 -type f -name "*.${EXT}" 2>/dev/null -exec cp {} "${DIRNAME}" \;
done < <(find $1 -mindepth 1 -type f -print 2>/dev/null | egrep -v "^.*\/\..*" | egrep "\."| cut -d '.' -f 2 | sort | uniq)

