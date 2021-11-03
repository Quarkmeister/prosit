#!/bin/bash
rm main.exe
rm *.o

g++ -c *.cpp
g++ -o main.exe *.o -lpigpio -lrt -lpthread -lboost_program_options

nice --15 ./main.exe -t 2