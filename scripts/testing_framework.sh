#!/bin/bash


if [ -e "$HOME/.trucovrc" ]
then
   echo "Removing trucov config file"
   rm $HOME/.trucovrc
fi

if [ -e "../output" ]
then
   echo "Clearing output directory"
   rm ../output/*.svg ../output/*.dot ../output/*.pdf ../output/*.trucov ../output/*.dump
else
   echo "Creating output directory"
   mkdir ../output/
fi

cmake -DCMAKE_BUILD_TYPE=Release -DCOVERAGE=Off -DDEBUGFLAG=On ../
make trucov
mv trucov ../output/ 
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=On -DDEBUGFLAG=Off ../
make all_unittest
./all_unittest
../output/trucov all_report -s ../include -s ../src -o ../output/ 


