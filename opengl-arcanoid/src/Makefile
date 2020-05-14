AUTHOR = Dmitry_Ivanov
NAME = arcanoid
BUILD = example
VERSION = 0.1

OBJECTS=$(addsuffix .o,$(basename $(shell ls *.cpp)))
WARNING_FLAGS = -Wall -Wextra -Weffc++ -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-parameter -Wstrict-overflow=5 -Wfloat-equal -Wshadow -Wconversion -Wsign-conversion -Wmissing-declarations -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo -Werror -Wpedantic -Wcast-qual
FORMATTING_FLAGS = -fno-pretty-templates -fmessage-length=80 -fdiagnostics-color=always
CFLAGS = ${WARNING_FLAGS} ${FORMATTING_FLAGS} -g -std=c++14 -pipe -frepo
LDLIBS = -lGL -lglut -lGLEW -lGLU -lX11 -lXi -lm -lrt -lpng
LDFLAGS =
CC = g++
TARGET = ar

all: ${OBJECTS}
	    ${CC} ${LDFLAGS} $^ ${LDLIBS} -o ${TARGET}

%.o: %.c
	    ${CC} ${CFLAGS} -c ${LDFLAGS} $< -o $@

makefile.dep: *.cpp *.h
	    for i in *.cpp; do g++ -MM "$${i}"; done > $@
include makefile.dep

clean:
	    rm ${OBJECTS} makefile.dep ${TARGET}

tar:	
	    make clean
			tar -cvzf ${NAME}_${BUILD}_${VERSION}.tar.gz *
