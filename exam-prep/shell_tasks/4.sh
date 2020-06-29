#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <file1> <file2>"
	exit 2
fi

FULLNAME1="/home/$(whoami)/${1}"
FULLNAME2="/home/$(whoami)/${2}"
if [ ! -f ${FULLNAME1} ] || [ ! -r ${FULLNAME1} ]
then
	echo "Error. ${FULLNAME1} is not a readable file"
	exit 1
fi
if [ ! -f ${FULLNAME2} ] || [ ! -r ${FULLNAME2} ]
then
	echo "Error. ${FULLNAME2} is not a readable file"
	exit 1
fi
LINES1=$(cat ${FULLNAME1} | grep "${1}" | wc -l)
LINES2=$(cat ${FULLNAME2} | grep "${2}" | wc -l)
WINNER="${FULLNAME1}"
if [ ${LINES2} -gt ${LINES1} ]
then
	WINNER="${FULLNAME2}"
fi

echo "Without year and singer"
cat ${WINNER} | cut -d '-' -f 2-
cat ${WINNER} | cut -d '-' -f 2- | sort > "${WINNER}.songs"
