#!/bin/sh

# This is a simple script for removing 
# bad runs files. Just giv it a file with 
# files (or numbers) in first 
# column.

file=../BADRUNS.txt

# regular QA runs (se-*.root)
cat $file | awk '{print $1}' | xargs rm -v
# CabanaBoy runs (<number>.root)
cat $file | awk '{print $1}' | sed 's/se-//' | xargs rm -v
