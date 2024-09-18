#!/bin/bash
set -e

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
CFGFILE=$SCPATH/mt5plots.cfg
OUTDIR=${WORKPATH}/plot/mt5

cd ${WORKPATH}

stdbuf -oL awk 'NF==2 {print $1,$2}' "${CFGFILE}" | while IFS=" " read -r SYMBOL TBIN;
do
 echo $SYMBOL $TBIN
 DATANAME=${SYMBOL}${TBIN}
 INPFILE=${WORKPATH}/data/${DATANAME}.csv
 INPFILE_UTF8=${WORKPATH}/data/${DATANAME}_utf8.csv

 scripts/chk_tbin.sh ${TBIN}
 ls -l ${INPFILE}
 dos2unix "${INPFILE}"
 root -l -q -b MakeMT5Plot.C+\(\"${OUTDIR}\",\"${INPFILE}\",\"${SYMBOL}\",\"${TBIN}\"\)
 ls -l ${OUTDIR}/${SYMBOL}_${TBIN}.root
done


root -l -q -b MakeMT5Plot.C+\(\"${OUTDIR}\",\"Overview\"\)
