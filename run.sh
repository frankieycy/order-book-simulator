#!/bin/bash
if [ -z "$1" ]; then
	file="main"
else
	file=$1
fi
g++ -std=c++11 -I ./lib run/${file}.cpp -o exe/${file}
if [ $? -eq 0 ]; then
	./exe/${file} 2>&1 | tee run.log
fi
