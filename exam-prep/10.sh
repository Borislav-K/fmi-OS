#!/bin/bash
cat /etc/passwd | awk -v GR=$(cat txt | tail -n +201 | head -n 1 | cut -d ':' -f 4) -F ':' '$4==GR {print $6":"$5}' |\
 cut -d ',' -f 1 | awk -F ':' '{print $2":"$1}'

