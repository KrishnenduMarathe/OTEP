# OTEP
Ouroboros Terminal Emulator Program

## Build Requirements

### 1. C++ Compiler
	Use any C++ compiler. Tested with GNU Compiler version 11
	If using clang, do change CXX variable in Makefile from CXX=g++ to CXX=clang++

### 2. Xlib development library

#### Debian-based distros
	sudo apt install libx11-dev

#### RPM-based distros
	sudo dnf/yum install libX11-devel

#### Arch-based distros
	sudo pacman -Syu libx11

## Installation
Either clone or download the archive of the repository. If you have downloaded the archive, extract the archive. Change directory to the project folder and open terminal and type following commands.

### Generate binary using:
	make

### If you want to install the application with a launcher in your application menu. This will also generate the binary for you, no need to use above command if you are using this. Use:
	make install

After using "make install", on some systems the application launcher show immediately or you might have to log in and log out again.

## Uninstallation
### Uninstall the application using:
	make uninstall

## Known Issues
	1. History file not implemented for multiple edo shell sessions. Might wreak havoc might not
	2. Large Commands don't work. Incorrect & Incomplete tokenization
	3. Number Keys on numpad might not work on some machine. (I got no clue how to fix)
	4. Cannot change directory as "cd" not implemented