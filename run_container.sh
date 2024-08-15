#!/bin/bash

SCFULLNAME=`realpath $0`
SCPATH=`dirname $SCFULLNAME`
USERNAME=$(id -u -n)

cd $SCPATH

docker run \
 --rm \
 --net host \
 -u ${UID}:$(id -g) \
 -w "/sv_dir" \
 -v ${PWD}/sv_dir:/sv_dir \
 -it jsroot_vista \
 /bin/bash


