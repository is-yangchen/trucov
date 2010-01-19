#!/bin/bash

rm *.o *gcno *gcda
gcc -fprofile-arcs -ftest-coverage -c ../src/cat.c -I../include/
g++ -fprofile-arcs -ftest-coverage -lgcov -o run ../src/main.cpp cat.o -I../include/

