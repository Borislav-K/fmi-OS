#!/bin/bash
#Да се напише shell скрипт, който намира броя на изпълнимите файлове в PATH.
#Hint: Предполага се, че няма спейсове в имената на директориите
#Hint2: Ако все пак искаме да се справим с този случай, да се разгледа IFS променливата и констуркцията while read -d


COUNT=0
while read -d ':' DIR
do
	COUNT=$(($COUNT+$(find "${DIR}" -mindepth 1 -type f | wc -l)))
done < <( echo $PATH)
echo "${COUNT} executables in PATH"
