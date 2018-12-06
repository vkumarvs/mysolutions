#
#CC = g++

#CFLAGS = -Wall -DTIXML_USE_STL

#all:    xmlreader

#xmlreader: lib
#	$(CC) $(CFLAGS) -o lib tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp

#clean:
#	rm -f xmlreader *.o

#$(CC) $(CFLAGS) -o $@ sctpsrvr.c -L/usr/local/lib -lsctp -pthread
#
# Makefile - VidScaleCommon
#
# Copyright Vidscale , 2014-2015
#
# History:
# Date            Who       Change
# 2014-05-26     vkumar     initial version
#
# g++ -o vidscalesim  VidScaleSigSimApp.cpp VidScaleSigSimThread.cpp # -lvidscalecomm -lpthread -I. -L lib

CXX=g++
AR=ar
RM	= rm -rf
ARFLAGS=-rcs
#WARNING_FLAGS+=-Wall -Wshadow -Wundef
WARNING_FLAGS+=-Wall -Wundef
DEBUG_FLAG=-g
OPTIMIZE_LEVEL=0

CXXFLAGS+=$(WARNING_FLAGS) $(DEBUG_FLAG) -O$(OPTIMIZE_LEVEL) -fno-strict-aliasing -DTIXML_USE_STL
CXXINCLUDES+=-I.


ARCHIVES=libtinyxml.a

libtinyxml_SRC=tinystr.cpp \
                    tinyxml.cpp \
                    tinyxmlerror.cpp \
                    tinyxmlparser.cpp

libtinyxml_OBJ=tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o


$(ARCHIVES):$(libtinyxml_SRC)
	$(CXX) $(CXXFLAGS) -c $(libtinyxml_SRC) $(CXXINCLUDES)   
	$(AR) $(ARFLAGS) $(ARCHIVES) $(libtinyxml_OBJ)


OBJECTS=$(wildcard *.o)


.PHONY: move
move: 
	mkdir obj 
	mkdir lib 
	mv *.o obj 
	mv *.a lib

.PHONY: clean 
clean: 
	$(RM) $(ARCHIVES) $(OBJECTS) 
	$(RM) obj lib 
 
clean-all: clean
