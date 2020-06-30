#!/bin/bash
#Да се напише shell скрипт, който получава при стартиране като параметър в
#командния ред идентификатор на потребител. Скриптът периодично (sleep(1))
#да проверява дали потребителят е log-нат, и ако да - да прекратява
#изпълнението си, извеждайки на стандартния изход подходящо съобщение.

if [ $# -ne 1 ]
then
	echo "Usage: $0 <UID>"
	exit 1
fi

USERNAME=$(cat /etc/passwd | awk -F ':' -v uid="${1}" '$3==uid {print $1}')
who | egrep -q "^${USERNAME}"
while  [ $? -ne 0  ]
do
	sleep 1
	who | egrep -q "^${USERNAME}"
done

echo "User $USERNAME logged in"
