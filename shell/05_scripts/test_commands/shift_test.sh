#!/bin/bash

index=1
for i in "$@"
do
	echo "Arg #$index = $i"
	index=$(($index+1))
done

index=1
shift 1
echo "Shifted once:"
for i in "$@"
do
	echo "Arg #$index = $i"
	index=$(($index+1))
done

index=1
shift 2
echo "Shifted twice:"
for i in "$@"
do
	echo "Arg#$index = $i"
	index=$(($index+1))
done
exit
