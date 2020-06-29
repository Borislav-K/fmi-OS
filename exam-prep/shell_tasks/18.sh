#!/bin/bash

UNAME_DIR_PAIRS=$(cut -d ':' -f 1,6 /etc/passwd | tr ':' ' ')
CANDIDATES=$(mktemp)
while read  USER HOMEDIR
do
	if [ -z "${USER}" ]
	then
		continue
	elif [ -z "${HOMEDIR}" ]
	then
		#echo "User ${USER} has no home directory record"
		continue
	elif [ ! -d "${HOMEDIR}" ]
	then
		#echo "${HOMEDIR} is not a directory"
		continue
	elif [ ! -r "${HOMEDIR}" ]
	then
		#echo "${HOMEDIR} is not readable"
		continue
	fi
	#MRC-->Most recently changed
	MRC_FILE=$(find ${HOMEDIR} -type f -printf "%T@ %p\n" 2>/dev/null | sort -nk1 | tail -n1)

	if [ -z "${MRC_FILE}" ]
	then
		#echo "No files found in ${HOMEDIR}"
		continue
	fi
	echo "${USER} ${MRC_FILE}" >> ${CANDIDATES}
done < <(echo "${UNAME_DIR_PAIRS}")



sort -t ' ' -nk2 ${CANDIDATES} | tail -n1 | cut -d ' ' -f 1,3


rm ${CANDIDATES}

