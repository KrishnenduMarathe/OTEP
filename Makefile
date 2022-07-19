# CXX COMPILER
CXX=g++
CXXFLAGS=-g -lX11 -lpthread

# Executable Name
EXEC=Otep

# Project Directory
PROJECT=$(shell pwd)
SRC=$(PROJECT)/src

all: build

build: config/icon.png config/otep.conf config/version
	$(CXX) // -o $(EXEC) $(CXXFLAGS)
	@echo "BUILD COMPLETE!\n\n"


clean:
	@rm $(EXEC)