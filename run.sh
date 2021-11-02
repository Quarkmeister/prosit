#!/bin/bash
rm main.exe
rm *.o

g++ -c *.cpp
g++ -o main.exe *.o -lpigpio -lrt -lpthread

nice --15 ./main.exe