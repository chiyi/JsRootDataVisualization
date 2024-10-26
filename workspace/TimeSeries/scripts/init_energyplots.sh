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
 exit 66  # EX_NOINPUT
fi

./MakeEnergyPlot.py ${OUTDIR} ${DATANAME}

