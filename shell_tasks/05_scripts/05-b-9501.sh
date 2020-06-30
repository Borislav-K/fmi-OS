#!/bin/bash
#Този път програмата ви ще приема само един параметър, който е измежду ("-r", "-b", "-g", "-x").
#Напишете shell script, който приема редовете от stdin и ги изпринтва всеки ред с редуващ се цвят. 
#Цветовете вървят RED-GREEN-BLUE и цветът на първия ред се определя от аргумента.
#Ако е подаден аргумент "-x", то не трябва да променяте цветовете в терминала (т.е., все едно сте извикали командата cat).

next_color() {
	if [ "${COLOR}" = "-r" ]
	then
		COLOR="-g"
	elif [ "${COLOR}" = "-g" ]
	then
		COLOR="-b"
	elif [ "${COLOR}" = "-b" ]
	then
		COLOR="-r"
	fi
}
if [ $# -ne 1 ]
then
	echo "Usage: $0 -r | -b | -g | -x"
	exit 1
fi

if [ "${1}" != "-r" ] && [ "${1}" != "-b" ] && [ "${1}" != "-g" ] && [ "${1}" != "-x" ]
then
	echo "Usage: $0 -r | -b | -g | -x"
	exit 1
fi

if [ "${1}" = "-x" ]
then
	while read LINE
	do
		echo $LINE
	done
else
	COLOR="${1}"	
	while read LINE
	do
		if [ "${COLOR}" = "-r" ]
		then
			echo -e "\033[0;31m${LINE}"
		elif [ "${COLOR}" = "-g" ]
		then
			echo -e "\033[0;32m${LINE}"
		else
			echo -e "\033[0;34m${LINE}"
		fi
		next_color
	done
fi

echo -ne '\033[0m'
