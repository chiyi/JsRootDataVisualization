#!/bin/bash
set -e

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
CFGFILE=$SCPATH/mt5plots.cfg

cd ${WORKPATH}

stdbuf -oL awk 'NF==2 {print $1,$2}' "${CFGFILE}" | while IFS=" " read -r SYMBOL TBIN;
do
 DATANAME=${SYMBOL}${TBIN}
 echo ${DATANAME}
 TARGETFILE=${WORKPATH}/data/${DATANAME}.csv
 DEMOFILE=${WORKPATH}/data/${DATANAME}_tail200.csv
 echo "generating plots from this data:"
 if [ -f "${TARGETFILE}" ];
 then
  ls -l ${TARGETFILE}
 else
  echo ${TARGETFILE} is not exist, try to copy old demo data. > "/dev/stderr"
  cp -p ${DEMOFILE} ${TARGETFILE}
 fi
done

