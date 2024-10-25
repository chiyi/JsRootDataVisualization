#!/bin/bash

SCFULLNAME=$(realpath $0)
SCPATH=$(dirname $SCFULLNAME)
WORKPATH=$(realpath $SCPATH/..)

# Define directories to clean
USERDIRS=("${WORKPATH}/func/user" "${WORKPATH}/plot/user")
SIZE_LIMIT=$((12 * 1024 * 1024 * 1024))    # 12GB

pattern="./*/*.json"
for diri in ${USERDIRS[@]};
do
 echo "Cleaning ${diri} ..."
 if ! cd "${diri}"; then
     echo "Failed to enter ${diri}, skipping..."
     continue
 fi

 while (( $(du -sb . | cut -f1) > ${SIZE_LIMIT} ));
 do
  file_rm=$(find ${pattern} -type f -printf '%T@ %p\n' | sort -n | head -1 | awk '{print $2}')
  if [ -z "${file_rm}" ]; then
   echo "pattern:${pattern} not found. Exiting the loop"
   break
  fi
  echo "Removing ${file_rm}"
  rm "${file_rm}"
 done

 echo "Size limit: ${SIZE_LIMIT} bytes. Current size: $(du -sh . | cut -f1)"
 echo
done

