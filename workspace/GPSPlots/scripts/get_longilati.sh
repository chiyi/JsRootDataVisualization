#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
SEL=${1:-"臺北市"}
OUTFILE=${2:-${WORKPATH}/data_Taipei/Taipei_edges.csv}


echo selecting ${SEL}
cd ${WORKPATH}
cat data_Taipei/COUNTY_MOI_1130718.gml | awk -v RS="<名稱>${SEL}</名稱>" '{if(NR>1) print}' | grep -m 1 "<gml:coordinates>" | awk -F "(<gml:coordinates>|</gml:coordinates>)" '{print $2}' | awk '{
 for(iNF=1; iNF<=NF; iNF++)
  printf "%s\n", $iNF;
}' > $OUTFILE

