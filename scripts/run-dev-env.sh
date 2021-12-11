#!/usr/bin/env bash

CWDIR=$(dirname $(dirname $(realpath -s $0)))

source $CWDIR/scripts/environment

command -v docker > /dev/null
if [ $? != 0 ]
then
    echo -e "Error: Docker isn't installed"
    exit 1
fi

if [ -z $(docker images -q $DEV_IMAGE) ]
then
    echo -e "Error: Required image is absent"
    exit 1
fi

docker run -it \
--rm \
--privileged \
--hostname=$USERNAME@dev \
--user=$USER_UID:$USER_GID \
--volume=${CWDIR}:${CWDIR}:rw \
--network=bridge \
--workdir=${CWDIR} \
${DEV_IMAGE}
