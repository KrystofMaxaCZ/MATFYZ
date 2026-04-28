#!/bin/bash

#set -oe pipeline

#"$@" > seznam_souboru.txt
#KDYBYCH POROVNAVAL POUZE JMENA SOUBORU
#uniq -d < echo "$@"

#sha256sum * > seznam_souboru.txt
#sha256sum * | cut -d" " -f1  | uniq -d
sha256sum * | sort | uniq -w 64 --group | cut -d " " -f 2
