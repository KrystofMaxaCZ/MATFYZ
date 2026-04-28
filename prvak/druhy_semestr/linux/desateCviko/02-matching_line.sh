#!/bin/bash

set -eou pipefail

#grep 'ABC' > /dev/null

#grep -q ABC

if grep -qx 'ABC'; then
	exit 0
else
	exit 1
fi

