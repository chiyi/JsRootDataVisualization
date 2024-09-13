#!/bin/bash

TBIN="$1"
awk -v tbin="${TBIN}" '
BEGIN{
 if (!(match(tbin, "^[M,H][0-9]+$") || match(tbin, "^(Daily|Weekly|Monthly)$")))
 {
  print "Requesting TBIN is not supported." > "/dev/stderr";
  exit 64;
 }
 exit 0;
}'
