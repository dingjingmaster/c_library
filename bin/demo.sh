#!/bin/bash

function success () {
    if [[ 0 -ne $1 ]]
    then
        exit -1
    fi
}

for filename in $(ls $1)
do
    $1$filename
    success $?
done

