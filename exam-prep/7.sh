#!/bin/bash
cat /etc/passwd | egrep "*:*:*:*:*$(cat /etc/passwd | cut -d ':' -f 5 | cut -d ' ' -f 2 | cut -d ',' -f 1 | egrep "a$"):" | cut -d ':' -f 1 | cut -c 3-5
