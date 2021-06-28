#!/bin/bash

# Vericar que la cantidad de argumentes sea la correcta
if [ $# -ne 2 ]; then
	echo "Uso $0 <P> <N>"
	exit
fi

#Modifique según el nombre de su programa
EXEC_FILE="./src/sumaParalela"

NUM_PROC=$1
NTOTAL=$2 #50000000 de float es suficiente para las pruebas

MPIOPTS=--oversubscribe
MPICXX=/usr/bin/mpirun

$MPICXX $MPIOPTS -n $NUM_PROC $EXEC_FILE --ntotal $NTOTAL
