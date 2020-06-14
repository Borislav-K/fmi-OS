#!/bin/bash
#Напишете shell script, който автоматично да попълва файла указател
#т предната задача по подадени аргументи: име на файла указател,
#пълно име на човека (това, което очакваме да е в /etc/passwd) 
# избран за него nickname. Файлът указател нека да е във формат:
#<nickname, който лесно да запомните> <username в os-server>
#римерно извикване:
#/pupulate_address_book myAddressBook "Ben Dover" uncleBen
#обавя към myAddressBook entry-то:
#uncleBen<username на Ben Dover в os-server>

if [ $# -ne 3 ]
then
	echo "Invalid number of arguments"
fi

if [ ! -w $1 ]
then
	echo "File $1 does not exist or is not writable"
fi

USERNAME=$(cat /etc/passwd | awk -v name="${2}" \
	-F ':' '$5==name {print $1}')
echo "$3 $USERNAME" >> $1

