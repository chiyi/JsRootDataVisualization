#!/bin/bash

SCFULLNAME=`realpath $0`
SCPATH=`dirname $SCFULLNAME`
WORKDIR=`realpath ${SCPATH}/../workspace`
export JSROOTSYS=`realpath ${SCPATH}/../jsroot`
LOGNAME="proc_SV.log"

cd ${WORKDIR}
date > ${JSROOTSYS}/${LOGNAME}
root -l -b SetSV.C++\("\"${LOGNAME}\""\) 2>&1 | tee -a ${JSROOTSYS}/${LOGNAME}

