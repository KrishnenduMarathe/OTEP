#pragma once
#ifndef TERMINAL_CONTROL_H_
#define TERMINAL_CONTROL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <thread>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

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
    bool isNumber(std::string str)
    {
        for (unsigned int itr = 0; itr < str.length(); itr++)
        {
            if (std::isdigit(str[itr]) == 0) { return false; }
        }

        return true;
    }

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

	// Draw Prompt
	bool draw_prompt = true;

	// Prompt
	std::string prompt;
    
    /* PROCESS VARIABLES */
    int width, height;
    char** buffer;

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
    uint8_t c_height, c_width;

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
void draw_width(int);
void draw();
void straighten_hw(int*, int*);

// event.cpp
void event_loop();

// command.cpp
void process_launch(std::string, int*, int*);

#endif // TERMINAL_CONTROL_H_