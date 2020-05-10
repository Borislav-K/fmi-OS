#!/bin/bash
cat passwd.txt | grep -i "inf" | cut -d ':' -f 1-5 | cut -d ',' -f 1 |\
egrep "a$" | cut -c 3-4 | sort | uniq -c  | sort -nr | head -n 1 
