#!/bin/bash

faktorial(){
   local cislo=$1

   test "$cislo" = "1" && echo 1 && return 
   local mensi=$(faktorial $((cislo - 1)))

   echo $((cislo * mensi))
}    

vysledek=$(faktorial $1)
echo "$vysledek"

#RESENI PODLE HORKYHO
seq -s " * " $1 | bc
seq "$1" | paste -s -d "*" | bc
