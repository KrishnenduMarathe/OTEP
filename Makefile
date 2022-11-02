# CXX COMPILER
CXX=g++
CXXFLAGS=-lX11 -lpthread

# Executable Name
LBITs=$(shell getconf LONG_BIT)
ifeq ($(LBITs), 64)
	EXEC=Otep.x64
else
	EXEC=Otep.x86
endif

PATH=$(shell pwd)
VERSION=$(shell cat $(PATH)/config/version)

# Project Objects
OBJECTS=src/command.o src/event.o src/init.o src/main.o src/utils.o

all: build

# Build Rules
build: $(OBJECTS)
	@echo "=> $(CXX) $(OBJECTS) -o $(EXEC) $(CXXFLAGS)"
	$(shell $(CXX) $(OBJECTS) -o $(EXEC) $(CXXFLAGS))

# Object Rules
src/%.o: src/%.cpp
	@echo "-> $(CXX) -c $^ -o $@"
	$(shell $(CXX) -c $^ -o $@)

# Debug Build
debug: clean
	@echo "=> $(CXX) -g src/*.cpp -o $(EXEC) $(CXXFLAGS)"
	$(shell $(CXX) -g src/*.cpp -o $(EXEC) $(CXXFLAGS))

# Install Rules
.PHONY: install
install: build
	@echo "[Desktop Entry]" > ~/.local/share/applications/otep.desktop
	@echo "Encoding=UTF-8" >> ~/.local/share/applications/otep.desktop
	@echo "Path=$(PATH)" >> ~/.local/share/applications/otep.desktop
	@echo "Version=$(VERSION)" >> ~/.local/share/applications/otep.desktop
	@echo "Type=Application" >> ~/.local/share/applications/otep.desktop
	@echo "Terminal=false" >> ~/.local/share/applications/otep.desktop
	@echo "Exec='$(PATH)/$(EXEC)'" >> ~/.local/share/applications/otep.desktop
	@echo "Name=OTEP" >> ~/.local/share/applications/otep.desktop
	@echo "Comment=Ouroboros Terminal Emulation Program" >> ~/.local/share/applications/otep.desktop
	@echo "Category=System;TerminalEmulator;" >> ~/.local/share/applications/otep.desktop
	@echo "Icon=$(PATH)/config/icon.png" >> ~/.local/share/applications/otep.desktop
	
	@echo " "
	@echo "Desktop Entry created for the current user"

# Uninstall Rules
.PHONY: uninstall
uninstall: clean
	$(shell rm ~/.local/share/applications/otep.desktop)
	@echo "Project Uninstalled!"

# Clean Rules
.PHONY: clean
clean:
	$(shell rm src/*.o)
	$(shell rm $(EXEC))
	@echo "Project Cleaned!"
