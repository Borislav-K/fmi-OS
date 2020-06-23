#!/bin/bash


if [ "$(whoami)" != "root" ]
then
	echo "Error. You are not root"
	exit 1
fi

TOTAL_ROOT_RSS=$(ps -u "root" -o rss= | awk '{sum+=$1} END {print sum}')

USERS=$(ps -eo user=, | sort | uniq | egrep -v "^root")
VALID_USERS=""
for USER in ${USERS}
do
	DIR=$(cut -d: -f 1,6 /etc/passwd | egrep "^${USER}:" | cut -d: -f2)
	#If they have no home directory
	if [ -z "${DIR}" ] || [ ! -d ${DIR} ]
	then
		VALID_USERS="${VALID_USERS} ${USER}"
		continue
	fi
	
	#If they do not own their directory
	OWNER=$(stat -c "%U" ${DIR})
	if [ "${OWNER}" != "${USER}" ]
	then
		VALID_USERS="${VALID_USERS} ${USER}"
		continue
	fi

	#If they cannot write to their directory
	ACCESS_RIGHTS=$(stat -c "%a" ${DIR})
	#User has no write access if the access rights' first digit is not 010(2),011(3),110(6) or 111(7)
	echo "${ACCESS_RIGHTS}" | egrep -vq "^[2367]"
	if [ $? -eq 0 ]
	then
		VALID_USERS="${VALID_USERS} ${USER}"
	fi
done

for USER in ${VALID_USERS}
do
	TOTAL_USER_RSS=$(ps -u "${USER}" -o rss= | awk '{sum+=$1} END {print sum}')
	if [ ${TOTAL_USER_RSS} -gt ${TOTAL_ROOT_RSS} ]
	then
		#Kill all processes
		PROCESSES=$(ps -u "${USER}" -o pid=)
		for PROCESS in ${PROCESSES}
		do
			kill ${PROCESS}
			sleep 1
			kill -9 ${PROCESS}
		done
	fi
done
