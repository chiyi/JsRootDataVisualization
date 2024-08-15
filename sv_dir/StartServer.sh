#!/bin/bash

SCFULLNAME=`realpath $0`
SCPATH=`dirname $SCFULLNAME`
export JSROOTSYS=${SCPATH}/jsroot
LOGNAME="proc_SV.log"


cd ${SCPATH}/workspace
date > ${JSROOTSYS}/${LOGNAME}
root -l -b SetSV.C++\("\"${LOGNAME}\""\) 2>&1 | tee -a ${JSROOTSYS}/${LOGNAME}

