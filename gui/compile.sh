#!/bin/bash

qmake
cp local_makefile Makefile
make all
mv tru_main trucov_gui
