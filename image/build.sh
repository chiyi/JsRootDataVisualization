#!/bin/bash

username=${USER-$(id -u -n)}
groupname=$(id -g -n)
userid=${UID-$(id -u)}
groupid=$(id -g)

docker build -t jsroot_vista --target BASE .

docker build \
 --build-arg USERNAME=${username} \
 --build-arg GROUPNAME=${groupname} \
 --build-arg UID=${userid} \
 --build-arg GID=${groupid} \
 -t jsroot_vista_${username} \
 --target USER \
 .
