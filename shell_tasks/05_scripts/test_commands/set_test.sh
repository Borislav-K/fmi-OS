#!/bin/bash
index=1
for i in "$@"
do
	echo "Arg #$index = $i"
	index=$(($index+1))
done

set a b c d e
echo "Issued \" set a b c d e \""

index=1
for i in "$@"
do
	echo "Arg #$index = $i"
	index=$(($index+1))
done
exit
