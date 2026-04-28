#!/bin/bash


find /opt/ -type f -path '*/microsoft/*' | while read filename; do
	jmeno="$(basename $filename | wc -c )"
	printf '%d %s\n' "$jmeno" "$filename"
done | sort -n | tail -n 1 | cut -d ' ' -f 2-


#lines=$(find /opt/ -type f -path '*/doc/*')
#while read line lines;
#echo "$lines"
