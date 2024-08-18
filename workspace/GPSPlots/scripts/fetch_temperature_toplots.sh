#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)
API_SITE="https://api.openweathermap.org/data/2.5/weather"
API_KEY=$(cat ${WORKPATH}/api_key)
OUTDIR=${WORKPATH}/data
MODE=${1:-"OLD"}
OLDTAG="2024Aug16_UTC_07H"

[ "${MODE}" == "UPDATE" ] && DTTAG=$(date '+%Y%b%d_%Z_%HH') || DTTAG=$OLDTAG
OUT_TABLE=${OUTDIR}/table_temperature_${DTTAG}.tsv
OUT_PLOT=${WORKPATH}/plot/temperature_${DTTAG}.root
OUT_JSON=${WORKPATH}/plot/temperature_${DTTAG}.json

cd ${WORKPATH}

if [ "${MODE}" == "UPDATE" ];
then
 rm ${OUT_TABLE} || echo create a new ${OUT_TABLE}
 for ((ilong=12148; $ilong<=12162; ilong+=2))
 do
  longitude=${ilong:0:3}.${ilong:3:2}
  for ((ilat=2496; $ilat<=2520; ilat+=2))
  do
   latitude=${ilat:0:2}.${ilat:2:2}
   echo $longitude , $latitude
   OUTFILE=${OUTDIR}/weather_${DTTAG}_Taipei_${longitude}_${latitude}.json
   curl -X GET "${API_SITE}?lat=${latitude}&lon=${longitude}&units=metric&appid=${API_KEY}" > $OUTFILE
   temperature=$(cat ${OUTFILE} | jq .main.temp)
   echo -e "${longitude}\t${latitude}\t${temperature}" >> ${OUT_TABLE}
  done
 done
fi

ls -l ${OUT_TABLE}

root -l -q -b MakePlot.C+\(\"${OUT_PLOT}\",\"${OUT_JSON}\",\"${DTTAG}\",\"${OUT_TABLE}\"\)

cp ${OUT_TABLE} ${OUTDIR}/table_temperature_latest.tsv
cp ${OUT_PLOT} ${WORKPATH}/plot/temperature_latest.root
cp ${OUT_JSON} ${WORKPATH}/plot/temperature_latest.json

