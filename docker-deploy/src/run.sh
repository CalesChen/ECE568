#!/bin/bash
make clean
make
echo 'start running proxy server...'
./main &
while true ; do continue ; done