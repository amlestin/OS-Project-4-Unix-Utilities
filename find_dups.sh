#!/bin/bash


# prints redundant files in directory

DIR=${1?Error: no directory chosen}
./lsrecr $DIR | sort | uniq -d -c
