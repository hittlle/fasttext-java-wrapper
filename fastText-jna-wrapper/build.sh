#!/bin/bash

#build dyanmic library
rm libfasttext.so
rm /lib/libfasttext.so

make clean;make so
cp libfasttext.so /lib

rm *.class
#test
javac -cp ".:jna-5.3.0.jar" FastTextJNAWrapper.java
java -cp ".:jna-5.3.0.jar" FastTextJNAWrapper
