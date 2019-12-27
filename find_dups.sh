#!/bin/bash


# prints redundant files in directory
# this solution respects the UNIX philosophy and makes use of minimal new code

DIR=${1?Error: no directory chosen}
./lsrecr $DIR | sort | uniq -d -c
