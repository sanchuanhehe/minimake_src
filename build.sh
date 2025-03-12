#!/bin/sh

echi "Building the project"
make build

echo "##############Running the project##############"
cp ./build_dir/minimake .
./minimake -v build

echo "Cleaning up"
rm minimake
