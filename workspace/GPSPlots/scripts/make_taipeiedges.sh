#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
INPFILE=${1:-${WORKPATH}/data_Taipei/Taipei_edges.csv}
OUTPLOT=${2:-${WORKPATH}/plot/Taipei_edges.root}


cd ${WORKPATH}
root -l -q -b MakeTaipeiEdges.C++\(\"${OUTPLOT}\",\"${INPFILE}\"\)

