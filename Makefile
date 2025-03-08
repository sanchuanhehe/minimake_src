# Description: Makefile for minimake
# Author: Sanchuanhehe
# Date: 2025-03-08

build: minimake

minimake: src/main.c
	gcc -o build/minimake src/main.c