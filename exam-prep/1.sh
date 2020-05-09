#!/bin/bash
FILE="philip-j-fry.txt"
echo "Броят на търсените редове е $(cat "${FILE}" | egrep -v "[a-w]+" |\
 egrep "[02468]+" | wc -l)"
