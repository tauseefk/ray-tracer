# if APPNAME is not defined, set it to the project dir name
ifndef APPNAME
    APPNAME = $(shell basename `pwd`)
endif

all:
	g++ -std=c++11 main.cpp -o $(APPNAME).o

OutputFile:all
	./$(APPNAME).o > output.ppm
	osascript -e 'display notification "Finished running!" with title "Ray Tracer"'

Output:all
	./$(APPNAME).o
	osascript -e 'display notification "Finished running!" with title "Ray Tracer"'

format:
	clang-format -i -style=file *.cpp *.h