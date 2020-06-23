#!/bin/bash
#Намерете колко физическа памет заемат осреднено всички процеси на потребителската група root. Внимавайте, когато групата няма нито един процес.

TOTAL_COUNT=$(ps -g "root" | tail -n +2 |wc -l)
if [ ${TOTAL_COUNT} -eq 0 ]
then
	echo "Error. root group has no processes running"
	exit 1
fi

TOTAL_MEMORY=$(ps -g "root" -o rss= | awk '{sum+=$1} END {print sum}')
AVG_MEMORY=$((${TOTAL_MEMORY}/${TOTAL_COUNT}))

echo "AVG_MEMORY : ${AVG_MEMORY}"

#One-Liner:
#echo "AVG_MEMORY: $(($(ps -g "root" -o rss= | awk '{sum+=$1} END {print sum}')/$(ps -g "root" | tail -n +2 | wc -l)))"
