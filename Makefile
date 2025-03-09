# Description: Makefile for minimake
# Author: Sanchuanhehe
# Date: 2025-03-08

build: build_dir/minimake

build_dir/minimake: build_dir/temp/main.o build_dir/temp/loger.o build_dir/temp build_dir/temp/mk_parser.o
	gcc -o build_dir/minimake src/main.c build_dir/temp/loger.o build_dir/temp/mk_parser.o -I include -Wall

build_dir/temp/loger.o: src/loger.c include/loger.h build_dir/temp
	gcc -c src/loger.c -o build_dir/temp/loger.o -I include -Wall

build_dir/temp/main.o: src/main.c build_dir/temp
	gcc -c src/main.c -o build_dir/temp/main.o -I include -Wall

build_dir/temp/mk_parser.o: src/mk_parser.c include/mk_parser.h build_dir/temp
	gcc -c src/mk_parser.c -o build_dir/temp/mk_parser.o -I include -Wall

build_dir: # create build_dir if not exist
	if [ ! -d build_dir ]; then mkdir build_dir; fi

build_dir/temp: build_dir
	if [ ! -d build_dir/temp ]; then mkdir build_dir/temp; fi

clean:
	rm -rf build_dir