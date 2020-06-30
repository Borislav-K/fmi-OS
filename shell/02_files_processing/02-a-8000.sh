#!/bin/bash
mkdir ~/bin2
find /bin/ -perm 777 -exec cp -r {} ~/bin2 \;
