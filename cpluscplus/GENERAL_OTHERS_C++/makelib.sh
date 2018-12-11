g++ -DVIPIN -g -c -I. lib_func.cpp

#create static library
ar rcs libfun.a  lib_func.o

#gcc -static main.c -L. -lmean -o statically_linked

#compiling main program and link things
g++ -DVIPIN lib_main_prog.cpp -L. -lfun -o lib_main.exe


