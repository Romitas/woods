#!/usr/bin/env bash

g++ -c main.cpp -std=c++14
g++ main.o -o app-woods -lsfml-graphics -lsfml-window -lsfml-system
./app-woods
