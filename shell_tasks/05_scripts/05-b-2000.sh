#!/bin/bash
#Да се напише shell скрипт, който приканва потребителя да въведе низ
#(име) и изпечатва "Hello, низ".

read -p "Enter your name:" NAME

echo "Hello, $NAME"
exit 0

