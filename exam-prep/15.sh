#!/bin/bash

if [ "$(whoami)" != "root" ]
then
	exit 0
fi

while read USER HOME_DIR
do
	if [ ! -d ${HOME_DIR} ] || sudo -u "${USER}" [ ! -w ${HOME_DIR} ]
	then
		echo "${USER}"
	fi
done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ')
