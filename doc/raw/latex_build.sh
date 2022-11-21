#!/bin/sh 
set -e
set -x 

pandoc --lua-filter diagram-generator.lua --template eisvogel $1.md -o output_$1.pdf
