#!/bin/bash
find ~pesho -mindepth 1 -links +1 2>/dev/null -exec stat -c "%n %Y %i" {} \;|\
sort -nk 2 | tail -n 1 | awk '{print $3}'
