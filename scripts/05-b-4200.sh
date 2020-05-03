#!/bin/bash
#Имате компилируем (a.k.a няма синтактични грешки) source file 
#на езика C. Напишете shell script, 
#който да покaзва колко е дълбоко най-дълбокото nest-ване (влагане)

if [ $# -ne 1 ]
then
	echo "Example usage: $0 <file_name>"
	exit 1
fi

if [ ! -r $1 ]
then
	echo "Error. File $1 does not exist or is not readable"
	exit 2
fi
MAX_NESTING=0
CURRENT_NESTING=0
BRACES=$(cat $1 | sed -r 's/[^{}]//g' | sed -r 's/(.)/\1\n/g')
#BRACES contains all the braces, each one on a new line
for word in $BRACES
do
	echo $word | grep "{" > /dev/null
	if [ $? -eq 0 ]
	then
		CURRENT_NESTING=$((${CURRENT_NESTING}+1))
		if [ $CURRENT_NESTING -gt $MAX_NESTING ]
		then
			MAX_NESTING=$CURRENT_NESTING
		fi
	fi
	echo $word | grep "}" > /dev/null
	if [ $? -eq 0 ]
	then
		CURRENT_NESTING=$((${CURRENT_NESTING}-1))
	fi
done

echo "The deepest nesting is $MAX_NESTING levels"
