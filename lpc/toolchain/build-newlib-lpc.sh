#!/bin/bash

PREFIX=/home/dhylands/lpc-4.1.1
DIR=`pwd`

LPC_INCLUDE=$PREFIX/arm-elf/include/arch/philips/newlib-lpc
LPC_LIB=$PREFIX/arm-elf/lib

export PATH=$PATH:$PREFIX/bin

cd $DIR/newlib-lpc
rm -f *.o *.a
make -f example.mak lib crt0.o

mkdir -p $LPC_INCLUDE
cp -f *.h $LPC_INCLUDE

cp libnewlib-lpc.a crt0.o $LPC_LIB
