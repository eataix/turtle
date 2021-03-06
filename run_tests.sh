#!/usr/bin/env bash

for i in tests/default/*.t
do
    ./turtle $i -s -l -o out.p &> /dev/null
    ./tools/DisASM ${i/.t/.p} out.asm > /dev/null
    sdiff out.p out.asm > /dev/null

    if [ $? -eq 0 ]
    then
        echo $i " passed"
    else
        echo $i " failed"
    fi
    rm -f out.p out.asm
done
