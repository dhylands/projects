#!/bin/bash

PREFIX=/home/dhylands/lpc-4.1.1
DIR=`pwd`

rm -rf $PREFIX
mkdir $PREFIX
cd $PREFIX
mkdir $PREFIX/info
mkdir $PREFIX/info/dir
mkdir $PREFIX/info/dir/dir

export PATH=$PATH:$PREFIX/bin

rm -rf $DIR/binutils
mkdir -p $DIR/binutils
cd $DIR/binutils

$DIR/binutils-2.17/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft  --enable-languages="c,c++"
make -j2
make install

rm -rf $DIR/gcc
mkdir -p $DIR/gcc
cd $DIR/gcc

$DIR/gcc-4.1.1/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft  --enable-languages="c,c++"
make -j2
make install

rm -rf $DIR/newlib
mkdir -p $DIR/newlib
cd $DIR/newlib

$DIR/newlib-1.14.0/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft
make -j2
make install

rm -rf $DIR/gcc2
mkdir -p $DIR/gcc2
cd $DIR/gcc2

$DIR/gcc-4.1.1/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft --enable-languages="c,c++" --with-newlib --with-headers=$PREFIX/newlib-1.14.0/newlib/libc/include
make -j2
make install

rm-rf $DIR/gdb
mkdir -p $DIR/gdb
cd $DIR/gdb

$DIR/gdb-6.6/configure --target=arm-elf --prefix=$PREFIX  --enable-interwork --enable-multilib --with-float=soft
make -j2
make install

rm -rf $DIR/insight
mkdir -p $DIR/insight
cd $DIR/insight

$DIR/insight-6.6/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft
make -j2
make install

