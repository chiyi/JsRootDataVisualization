#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
DATANAME="data/global-energy-substitution.csv"
OUTPLOT="plot/Energy_Consumption.root"

cd ${WORKPATH}

if [ ! -f "${DATANAME}" ];
then
 echo ${DATANAME} is missing.
 exit 13
fi

./MakePlot.py ${OUTPLOT} ${DATANAME}

