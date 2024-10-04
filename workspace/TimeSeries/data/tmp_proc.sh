#!/bin/bash

TARGETS=(EURUSDH1 GBPUSDH1 NVDADaily NVDAH1 USDCADH1 USDCHFH1 USDJPYH1 XAGUSDH1 XAUUSDH1 XPTUSDH1)

for iX in ${TARGETS[@]};
do
 echo $iX
 dos2unix ${iX}.csv
 cat ${iX}.csv | tail -200 > ${iX}_tail200.csv
done
