#!/bin/bash
find ~velin -mindepth 1 -inum $(find ~velin -mindepth 1 -printf "%T@ %i\n" 2>/dev/null | sort -n | tail -n 1 | awk '{print $2}') 2>/dev/null |\
	egrep -o '.' | fgrep '/' | wc -l

