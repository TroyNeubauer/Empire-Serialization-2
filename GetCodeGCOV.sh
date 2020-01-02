#!/bin/bash
for filename in `find . | egrep '\.o'`; 
do
	gcov-9 $filename
done