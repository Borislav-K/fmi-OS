#!/bin/bash
cat /etc/passwd | egrep "$(find /home/SI -mindepth 1 -maxdepth 1 -type d 2>/dev/null -exec stat -c "%n %Z" {} \; |\
       	awk '$2>1551168000 && $2<1551176100 {print $1}')$" | cut -d ':' -f 1,5 | cut -c 2- | cut -d ',' -f 1 | tr ':' '\t'
