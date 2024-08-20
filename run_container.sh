#!/bin/bash

SCFULLNAME=`realpath $0`
SCPATH=`dirname $SCFULLNAME`
USERNAME=$(id -u -n)

cd $SCPATH

docker run \
 --rm \
 --net host \
 -w "/sv_dir" \
 -u ${UID}:$(id -g) \
 -v ${PWD}/Launcher:/sv_dir/Launcher \
 -v ${PWD}/workspace:/sv_dir/workspace \
 -it jsroot_vista_${USERNAME} \
 /bin/bash

