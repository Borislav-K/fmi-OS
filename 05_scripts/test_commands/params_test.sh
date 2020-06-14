#!/bin/bash
index=1
echo "Listing arguments with \"\$*\": "
for a in "$*"
do
	echo "arg $index = $a"
	index=$(($index+1))
done

index=1
echo "Listing arguments with \"\$@\": "
for a in "$@"
do
	echo "arg $index = $a"
	index=$((index+1))
done

index=1
echo "Listing arguments with \$*:"
for a in $*
do
	echo "arg $index = $a"
	index=$(($index+1))
done

index=1
echo "Listing arguments with \$@: "
for a in $@
do
	echo "arg $index = $a"
	index=$(($index+1))
done

echo "\$@: $@"
echo "\"\$@\": $@"
exit

