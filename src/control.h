// AUTHOR: Krishnendu Marathe
// PROGRAM: OTEP
 
/*
**                                                        OTEP
**                                                        ----
**                                          Ouroboros Terminal Emulator Program
**
**                                                       License
**                                                       -------
**
**        This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef TERMINAL_CONTROL_H_
#define TERMINAL_CONTROL_H_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <X11/keysym.h>

// Singleton Design Pattern for the Control Class
class TerminalControl
{
private:
    static TerminalControl instance;

    // Constructor declared private for the pattern to work
    TerminalControl() {}

    /* PRIVATE METHOD DECLARATIONS */
    
	// utils.cpp
	bool isNumber(std::string);

public:
    // Determine Successful Initialization
    bool init = false;

    /* CONFIG VARIABLES */
    bool dark;
    std::string font_name = "";
    std::string custom_color = "";
    std::string font_dark_color = "";
    std::string font_custom_color = "";
    std::string t_width = "", t_height = "";

    bool history;
    unsigned int h_lines = 0;

    // Debug
    bool debug = false;
	bool advance_debug = false;

	// Exit Marker
    bool exit_loop = false;

	// Path environment variable
	std::vector<std::string> path;

	// Draw Prompt
	bool draw_prompt = true;

	// Prompt
	std::string prompt;
    
    /* PROCESS VARIABLES */
    int width, height;
    char* buffer;

    // X11
    Display* display;
    int screen;
    Window root, main;
    GC gc;
    XFontStruct* font;
    Colormap cmap;

    unsigned long whitepx, blackpx;
    unsigned int resolution_x = 0, resolution_y = 0;

    XColor u_color, s_color;
    XEvent event;

    // Character Dimension
    int c_height, c_width;

    // Terminal Dimension in Characters
    int charWidth = 0, charHeight = 0;

    // BG & FG Pixels
    unsigned long bg, fg;

	// Keyboard Triggers
	struct
	{
		bool caps = false;
    	bool shift = false;
	    bool control = false;
    	bool alt = false;
	} kb_triggers;

    // Set copy constructor to Delete
    TerminalControl(const TerminalControl&) = delete;

    static TerminalControl& getInstance() { return instance; }

    /* PUBLIC METHOD DECLARATIONS */
	
	// init.cpp
    void initiate();
	bool read_config();
	void terminate();

	// keyboard.h
    char getKeyInChar();

};

/* FUNCTION PROTOTYPES */

// utils.cpp
void display_resize();
void init_display();
void draw_width(int);
void draw();
void straighten_hw(int*, int*);

// event.cpp
void event_loop();

// command.cpp
void process_launch(std::string, int*, int*);

#endif // TERMINAL_CONTROL_H_
