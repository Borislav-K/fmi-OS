#!/bin/bash
#Намерете колко физическа памет заемат всички процеси на потребителската група root.

TOTAL_RSS=$(ps -e -G "root" -o drs= | awk '{sum+=$1} END {print sum}')
echo "Total RSS: ${TOTAL_RSS}"
