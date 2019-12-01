#!/bin/bash
set -e

USAGE="bash run.sh DAY"

if [[ $# != 1 ]]
then
    echo "$USAGE"
    exit 1
fi

DAY="$(printf "%02d" "$1")"
g++ -std=c++17 "./day$DAY/solution$DAY.cpp" -o "./day$DAY/solution$DAY.exe"
"./day$DAY/solution$DAY.exe" "./day$DAY/input$DAY.txt"
