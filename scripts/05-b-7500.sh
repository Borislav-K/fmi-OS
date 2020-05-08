#!/bin/bash
#Напишете shell script guess, която си намисля число, което вие трябва да познате. В зависимост от вашия отговор,
# програмата трябва да ви казва "надолу" или "нагоре", докато не познате числото.
# Когато го познаете, програмата да ви казва с колко опита сте успели.

RAND=$(( (RANDOM % 100) + 20  ))
TOTALTRIES=0
while true 
do
	read -p "Guess: " INPUT
	echo "${INPUT}" | egrep -v "^[0-9]+$"
	if [ $? -eq 0 ]
	then
		echo "Please enter a number!"
		continue
	fi
	TOTALTRIES=$((${TOTALTRIES}+1))
	if [ ${INPUT} -eq ${RAND} ]
	then
		echo "You win!!! It took you ${TOTALTRIES} tries"
		exit 0
	fi
	if [ ${INPUT} -gt ${RAND} ]
	then
		echo "Lesser"
		continue	
	fi
	echo "Greater"
done

