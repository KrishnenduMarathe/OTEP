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
# Project Objects
OBJECTS=src/command.o src/event.o src/init.o src/main.o src/utils.o

all: build config/icon.png config/otep.conf config/version

# Build Rules
build: $(OBJECTS)
	@echo "\n$(CXX) $(OBJECTS) -o $(EXEC) $(CXXFLAGS)"
	$(shell $(CXX) $(OBJECTS) -o $(EXEC) $(CXXFLAGS))

# Object Rules
src/%.o: src/%.cpp
	@echo "$(CXX) -c $^ -o $@"
	$(shell $(CXX) -c $^ -o $@)

# Debug Build
debug: clean
	@echo "\n$(CXX) -g src/*.cpp -o $(EXEC) $(CXXFLAGS)"
	$(shell $(CXX) -g src/*.cpp -o $(EXEC) $(CXXFLAGS))

# Install Rules
.PHONY: install
install: build
	@echo "[Desktop Entry]\nEncoding=UTF-8\nPath=$(PATH)\nVersion=\`cat $(PATH)/config/version\`\nType=Application\nTerminal=false\nExec='$(PATH)/Otep'\nName=OTEP\nComment=Ouroboros Terminal Emulation Program\nCategory=Utility;\nIcon=$(PATH)/config/icon.png" > ~/.local/share/applications/otep.desktop

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