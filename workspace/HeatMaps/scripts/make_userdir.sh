#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
dir_base=$1
user=${2:-default_name}

cd ${WORKPATH}

if [ -d "${dir_base}" ];
then
 if [ ! -d "${dir_base}/${user}" ];
 then
  mkdir ${dir_base}/${user}
 fi
fi

ls -ld ${dir_base}/${user}
