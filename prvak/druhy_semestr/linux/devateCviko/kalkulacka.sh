#!/bin/bash

set -eou pipefail

value=0
while [ "$1" -ne 0 ];
do
	if [ "$value" -eq 0 ]; then
		value="$1"
		echo "$value"
	else
		value=$(("$value" + "$1"))
	fi
	echo "$value"
done
