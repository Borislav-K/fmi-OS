#!/bin/bash
#Изведете имената на потребителите, които не са логнати в момента, но имат живи процеси

USERS_WITH_ALIVE_PROCESSES=$(ps -eo user= | sort | uniq)
LOGGED_USERS=$(who | awk '{print $1}' | sort | uniq)
for USER in ${USERS_WITH_ALIVE_PROCESSES}
do
	echo "${LOGGED_USERS}" | egrep -q "^${USER}"
	if [ $? -ne 0 ]
	then
		echo "${USER}"
	fi
done
