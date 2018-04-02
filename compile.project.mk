# if APPNAME is not defined, set it to the project dir name
ifndef APPNAME
    APPNAME = $(shell basename `pwd`)
endif

all:
	g++ -std=c++11 main.cpp -o $(APPNAME).o

OutputFile:all
	./$(APPNAME).o > colors.ppm

Output:all
	./$(APPNAME).o