#!/bin/bash

set -ueo pipefail

shall_print_size=true
shall_print_type=true
print_size() {
    if $shall_print_size; then
        if [ -d "$1" ]; then
	    printf ' [dir]'
	else
	    stat -c " [%s]" "$1"
	fi
    fi

}

print_tree() {
	local top="$1"
	local indent="$2"
	#size=$(ls -lah "$1" | cut -d ' ' -f 5 )

	printf '%s%q%s\n' "$indent" "$top" "$( print_size "$top" )"
	if [ -d "$top" ]; then
	   (
	   	cd "$top"
		for f in *; do
		   test -e "$f" || continue
		   print_tree "$f" "${indent}  "
		done
	   )
	fi
}


while getopts "sF" opt; do
    case "$opt" in
       s) shall_print_size=true ;;
       F) shall_print_type=true ;;
       *) exit 1 ;; 
    esac
done
shift $$(( OPTIND - 1 ))

print_tree . ""

#for i in *; do
#	if [ $i -d ]; then
#		ls $i
#	fi
#

