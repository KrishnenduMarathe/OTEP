# CXX COMPILER
CXX=g++
CXXFLAGS=-lX11 -lpthread

# Executable Name
EXEC=Otep

PATH=$(shell pwd)
# Project Objects
OBJECTS=src/event.o src/init.o src/main.o src/utils.o

all: build

# Build Rules
build: config/icon.png config/otep.conf config/version $(OBJECTS)
	$(CXX) -o $(EXEC) $(OBJECTS) $(CXXFLAGS)

src/%.o: src/%.cpp
	$(CXX) -o $@ -c $^

# Install Rules
.PHONY: install
install: build
	@echo "[Desktop Entry]\nEncoding=UTF-8\nPath=$(PATH)\nVersion=\`cat $(PATH)/config/version\`\nType=Application\nTerminal=false\nExec='$(PATH)/Otep'\nName=OTEP\nComment=Ouroboros Terminal Emulation Program\nCategory=Utility;\nIcon=$(PATH)/config/icon.png" > ~/.local/share/applications/otep.desktop

# Uninstall Rules
.PHONY: uninstall
uninstall: clean
	@rm ~/.local/share/applications/otep.desktop

# Clean Rules
.PHONY: clean
clean:
	@rm src/*.o
	@rm $(EXEC)