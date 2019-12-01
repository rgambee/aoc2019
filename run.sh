#!/bin/bash
set -e

USAGE="bash run.sh DAY"

if [[ $# != 1 ]]
then
    echo "$USAGE"
    exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")"

DAY="$(printf "%02d" "$1")"
g++ -std=c++17 -Wall -Wextra -Werror -o "./day$DAY/solution$DAY.exe" -I "./utils/" \
    "./day$DAY/solution$DAY.cpp" "./utils/utils.cpp"
"./day$DAY/solution$DAY.exe" "./day$DAY/input$DAY.txt"
