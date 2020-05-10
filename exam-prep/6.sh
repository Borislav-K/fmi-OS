#!/bin/bash
#a)
find . -mindepth 1 -type f -size 0 -exec rm {} \;
#b)
find ~ -mindepth 1 -type f -printf "%p %s\n" | sort -rnk 2 | head -n 5 |cut -d ' ' -f 1 | xargs rm
