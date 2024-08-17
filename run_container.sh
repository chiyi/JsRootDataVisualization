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
 -v ${PWD}/scripts:/sv_dir/scripts \
 -v ${PWD}/workspace:/sv_dir/workspace \
 -it jsroot_vista \
 /bin/bash

