#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
FILECFG=$1
OUTPREFIX=$2

cd ${WORKPATH}

if [ -z "${FILECFG}" ] || [ -z "${OUTPREFIX}" ] || [ ! -d "$(dirname "${OUTPREFIX}")" ];
then
 echo "Error: Missing required file configuration or output directory setup."
 exit 64  # EX_USAGE
fi

if [ ! -f "${FILECFG}" ];
then
 echo "config_file: ${FILECFG} is missing."
 exit 66  # EX_NOINPUT
fi


root -l -q -b Gen_Sim7layers.C+\(\"${OUTPREFIX}\",\"${FILECFG}\"\)

