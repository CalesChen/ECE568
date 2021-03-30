#!/bin/bash
#sudo su - postgres & psql & CREATE DATABASE MATCH_ENGINE
make clean
make all
echo 'start running server...'
./server
while true ; do continue ; done