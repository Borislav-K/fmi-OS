#!/bin/bash
#Намерете PID и командата на процеса, който заема най-много виртуална памет в системата.

ps -eo pid,cmd --sort +vsz | tail -1
