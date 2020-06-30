#!/bin/bash
#Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
#Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.

#Да се дефинира променлива BACKUP_DIR (или друго име), в която:
#- изтритите файлове се компресират и запазват
#- изтритите директории се архивират, комприсират и запазват
#- имената на файловете е "filename_yyyy-mm-dd-HH-MM-SS.{gz,tgz}", където filename е оригиналното
# име на файла (директорията) преди да бъде изтрит

#а) Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно и 
#съответно да се запазят в BACKUP_DIR

timestamp() {
	date +"%Y-%m-%d-%H-%M-%S"
}

compress_and_save () {
	FILE=$1
	TIMESTAMP=$(timestamp)
	gzip -c ${FILE} > "${BACKUP_DIR}/${FILE}_${TIMESTAMP}.gz"
}

archive_compress_save () {
	DIR=$1
	TIMESTAMP=$(timestamp)
	tar -czf "${BACKUP_DIR}/${DIR}_${TIMESTAMP}.tgz" "${DIR}"
}
if [ $# -eq 0 ]
then
	echo "Usage: $0 [-r] [<filename>]+"
	exit 1
fi

REC="f"
if [ "${1}" = "-r" ]
then
	REC="t"
	shift 1
fi

for FILE in "$@"
do
	if [ -f ${FILE} ]
	then
		compress_and_save "${FILE}"
		rm "${FILE}"
	elif [ -d ${FILE} ]
	then
		ITEMCOUNT=$(find ${FILE} -mindepth 1 -maxdepth 1 2>/dev/null| wc -l)
		if [ ${ITEMCOUNT} -eq 0 ]
		then
			archive_compress_save "${FILE}"
			rmdir "${FILE}"
		elif [ "${REC}" = "t" ]
		then
			archive_compress_save "${FILE}"
			rm -rf "${FILE}"
		fi
	else
		echo "${FILE} is not a valid file/directory"
	fi
done



