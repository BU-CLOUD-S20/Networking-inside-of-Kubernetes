#!/bin/bash

if [ -z $1 ]; then
        #echo "you must input a port"
        exit 0
fi

PID=$(netstat -nlp | grep ":$1" | awk '{print $7}' | awk -F '[ / ]' '{print $1}')

if [ $? != 0 ]; then
        exit 0
fi

kill -9 ${PID} > /dev/null 2>&1