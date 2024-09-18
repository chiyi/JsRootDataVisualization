#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
DATANAME="data/global-energy-substitution.csv"
OUTDIR="plot/energy"

cd ${WORKPATH}

if [ ! -f "${DATANAME}" ];
then
 echo ${DATANAME} is missing.
 exit 13
fi

./MakeEnergyPlot.py ${OUTDIR} ${DATANAME}

