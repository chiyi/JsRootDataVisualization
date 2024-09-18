#!/bin/bash

SCFULLNAME=`realpath $0`
SCPATH=`dirname $SCFULLNAME`
export WORKDIR=`realpath ${SCPATH}/../workspace`
export JSROOTSYS=`realpath ${SCPATH}/../jsroot`
LOGNAME="proc_SV.log"

cd ${WORKDIR}
#cp ${SCPATH}/htaccess ${JSROOTSYS}/.htaccess
${SCPATH}/scripts/insert_files.sh ${SCPATH}/insert_files/GPSPlots_files.txt
${SCPATH}/scripts/insert_files.sh ${SCPATH}/insert_files/TimeSeriesPlots_files.txt

date > ${JSROOTSYS}/${LOGNAME}
root -l -b SetSV.C++\("\"${LOGNAME}\""\) 2>&1 | tee -a ${JSROOTSYS}/${LOGNAME}

