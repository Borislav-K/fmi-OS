#!/bin/bash
#Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
#Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.
#За всеки изтрит файл (директория) скриптът добавя ред във log файл с подходящо съобщение.

#а) Името на log файла да се чете от shell environment променлива, която сте конфигурирали във вашия .bashrc.
#б) Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно.
#в) Добавете timestamp на log съобщенията във формата: 2018-05-01 22:51:36

#Примери:
#$ export RMLOG_FILE=~/logs/remove.log
#$ ./rmlog -r f1 f2 f3 mydir/ emptydir/
#$ cat $RMLOG_FILE
#[2018-04-01 13:12:00] Removed file f1
#[2018-04-01 13:12:00] Removed file f2
#[2018-04-01 13:12:00] Removed file f3
#[2018-04-01 13:12:00] Removed directory recursively mydir/
#[2018-04-01 13:12:00] Removed directory emptydir/

if [ $# -eq 0 ]
then 
	echo "Error. At least 1 argument required"
	exit 1
fi

REC="f"
if [ "${1}" = "-r" ]
then
	REC="t"
	shift 1
fi
if [ ! -e ${LOGFILE} ]
then
	touch "${LOGFILE}"
fi
for FILE in "$@"
do
	echo -n $(date +"[%Y-%m-%d %H:%M:%S] ") >>${LOGFILE}
	if [ -f ${FILE} ]
	then
		rm ${FILE} 2>/dev/null
		if [ $? -eq 0 ]
		then
			echo " File ${FILE} successfully deleted" >> ${LOGFILE}
		else
			echo " File ${FILE} could not be deleted" >> ${LOGFILE}
		fi
	elif [ -d ${FILE} ]
	then
		rmdir ${FILE} 2>/dev/null
		if [ $? -eq 0 ]
		then
			echo " Directory ${FILE} successfully deleted" >> ${LOGFILE}
		elif [ "${REC}" = "f" ]
		then
			echo " Directory ${FILE} could not be deleted" >> ${LOGFILE}
		else 
			rm -rf ${FILE} 2>/dev/null
			echo " Directory ${FILE} recursively deleted" >> ${LOGFILE}
		fi
	else 
		echo " ${FILE} is not a valid file or directory" >> ${LOGFILE}
	fi
done 
