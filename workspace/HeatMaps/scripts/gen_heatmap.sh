#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
FILECFG=$1
OUTPLOT=$2

cd ${WORKPATH}

if [ -z "${FILECFG}" ] || [ -z "${OUTPLOT}" ] || [ ! -d "$(dirname "${OUTPLOT}")" ];
then
 echo "Error: Missing required file configuration or output directory setup."
 exit 64  # EX_USAGE
fi

if [ ! -f "${FILECFG}" ];
then
 echo "config_file: ${FILECFG} is missing."
 exit 66  # EX_NOINPUT
fi


root -l -q -b Gen_HeatMap.C+\(\"${OUTPLOT}\",\"${FILECFG}\"\)

