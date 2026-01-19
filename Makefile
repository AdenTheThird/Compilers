#**************************************
# Makefile
#
# Makefile for lang compiler
#
# Author: Phil Howard 
# phil.howard@oit.edu
#
# Date: Jan. 12, 2016
#

COPTS=-Wall -g -O0 -std=c++11

OBJS=main.o langlex.o cSymbolTable.o

all: lang

clean:
	rm -f $(OBJS) langlex.c lang out.xml out2.xml

langlex.c: lang.l
	flex -o langlex.c lang.l  # or flex -+ -o langlex.c lang.l for C++

.c.o:
	g++ $(COPTS) -c $< -o $@

.cpp.o:
	g++ $(COPTS) -c $< -o $@

langlex.o: langlex.c
	g++ $(COPTS) -c $< -o $@

main.o: main.cpp langlex.c
	g++ $(COPTS) -c main.cpp -o main.o

lang: $(OBJS)
	g++ $(OBJS) -o lang

