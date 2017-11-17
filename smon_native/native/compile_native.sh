#!/bin/sh

current_dir=$(dirname $(readlink -f "$0"))
cd ${current_dir}/smon_native

make

mkdir -p ../bin
mv *.so ../bin/

