#!/bin/bash

later_version() {
	V1=$(echo "${1}" | sed -r 's/vmlinuz-(.*)\..*\..*-.*/\1/')
	V2=$(echo "${2}" | sed -r 's/vmlinuz-(.*)\..*\..*-.*/\1/')
	if [ ${V1} -gt ${V2} ]
	then
		echo "1"
		exit
	elif [ ${V1} -lt ${V2} ]
	then
		echo "2"
		exit
	fi

	MAJOR_R1=$(echo "${1}" | sed -r 's/vmlinuz-.*\.(.*)\..*-.*/\1/')
	MAJOR_R2=$(echo "${2}" | sed -r 's/vmlinuz-.*\.(.*)\..*-.*/\1/')
	if [ ${MAJOR_R1} -gt ${MAJOR_R2} ]
	then
		echo "1"
		exit
	elif [ ${MAJOR_R1} -lt ${MAJOR_R2} ]
	then
		echo "2"
		exit
	fi

	MINOR_R1=$(echo "${1}" | sed -r 's/vmlinuz-.*\..*\.(.*)-.*/\1/')
	MINOR_R2=$(echo "${2}" | sed -r 's/vmlinuz-.*\..*\.(.*)-.*/\1/')

	if [ ${MINOR_R1} -ge ${MINOR_R2} ]
	then
		echo "1"
		exit
	fi

	echo "2"
}
if [ $# -ne 2 ]
then
	echo "Usage: $0 <dir> <str>"
	exit 1
fi

if [ ! -d $1 ]
then
	echo "Error. $1 must be a directory"
	exit 2
fi

if [ ! -r $1 ]
then
	echo "Error. $1 must be a readable directory"
	exit 2
fi

VALID_FILES=$(find $1 -maxdepth 1 -type f -regex "${1}/vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${2}" -printf "%f\n")
if [ -z "${VALID_FILES}" ]
then
	echo "No files found"
	exit 3
fi

LATEST_FILE=$(echo "${VALID_FILES}" | head -1)
while read FILE
do
	IS_LATER=$(later_version ${FILE} ${LATEST_FILE})
	if [ "${IS_LATER}" = "1" ]
	then
		LATEST_FILE=${FILE}
	fi
done < <( echo "${VALID_FILES}" | tail -n +2)

echo ${LATEST_FILE}

