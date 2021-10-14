#!/usr/bin/env bash

CONTR_PORT=2222
CONTR_NAME=sensority-remote-env
IMAGE_NAME=my/sensority-remote-dev

command -v docker > /dev/null
if [ $? != 0 ]
then
    echo -e "Error: Docker isn't installed"
    exit 1
fi

start()
{
    ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:$CONTR_PORT" > /dev/null 2>&1
    docker run -d -p127.0.0.1:$CONTR_PORT:22 --cap-add sys_ptrace --name $CONTR_NAME $IMAGE_NAME
}

stop()
{
    docker stop $CONTR_NAME
}


if [ "$1" = "start" ]; then
    start
    exit 0
fi

if [ "$1" = "stop" ]; then
    stop
    exit 0
fi

echo -e "Error: Argument is missing or invalid"
