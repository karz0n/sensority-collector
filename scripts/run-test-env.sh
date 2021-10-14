#!/usr/bin/env bash

CWDIR=$(dirname $(dirname $(realpath -s $0)))
IMAGE=my/sensority-dev

command -v docker > /dev/null
if [ $? != 0 ]
then
    echo -e "Error: Docker isn't installed"
    exit 1
fi

if [ -z $(docker images -q ${IMAGE}) ]
then
    echo -e "Error: Required image is absent"
    exit 1
fi

docker run -it \
--rm \
--privileged \
--user $(id -u):$(id -g) \
--volume=${CWDIR}:${CWDIR}:rw \
--network=bridge \
--workdir=${CWDIR} \
${IMAGE} /bin/bash
