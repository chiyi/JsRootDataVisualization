#!/bin/bash

USERNAME=$(echo $1 | awk -F ":" '{print $1}')
GROUPNAME=$(echo $1 | awk -F ":" '{print $2}')
USERID=$(echo $2 | awk -F ":" '{print $1}')
GROUPID=$(echo $2 | awk -F ":" '{print $2}')

echo $USERNAME
echo $GROUPNAME
echo $USERID
echo $GROUPID

if getent group "${GROUPID}" > /dev/null 2>&1; then
  echo "GROUPID ${GROUPID} exists"
else
  echo "GROUPID ${GROUPID} does not exist. adding..."
  groupadd -g ${GROUPID} ${GROUPNAME}
  getent group "${GROUPID}"
fi

if getent passwd "${USERID}" > /dev/null 2>&1; then
  echo "USERID ${USERID} exists"
else
  echo "USERID ${USERID} does not exist. adding..."
  useradd -m -u ${USERID} -g ${GROUPID} --create-home --home-dir /home/${USERNAME} ${USERNAME}
  getent passwd "${USERID}"
fi

