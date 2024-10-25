#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
FILECFG=${1:-"${SCPATH}/Base_Functions_2D.cfg"}

cd ${WORKPATH}

if [ ! -f "${FILECFG}" ];
then
 echo ${FILECFG} is missing.
 exit 13
fi

root -l -q -b Test_Funcs2D.C+\(\"${FILECFG}\"\)
