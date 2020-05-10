#!/bin/bash
find . -mindepth 1 -printf "%f %n\n" | sort -rnk 2 | head -n 5 |\
 awk '{print $1}'
