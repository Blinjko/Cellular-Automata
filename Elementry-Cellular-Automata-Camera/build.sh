#!/bin/sh


# Script to build the program
# Can use CLANG to probably


gcc src/error.c src/grid.c src/rules.c src/main.c -L ./lib/ -lraylib -lm -o Elementry-Automata
