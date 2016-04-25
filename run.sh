#!/bin/sh

( cd build && make )

FOLDER1="$1"
FOLDER2="$2"

echo "================== Testing $FOLDER1 against $FOLDER2 =================="

for bin1 in ${FOLDER1}/*.bin
do
    bn=$(basename "$bin1")
    bin2=${FOLDER2}/${bn}

    if [ ! -f "$bin2" ]
    then
        continue
    fi

    echo "Testing $bn"
    build/democomp "$bin1" "$bin2"

    if [ $? = 0 ]
    then
        echo "Passed!"
    else
        echo "Failed!"
    fi
    echo
done

