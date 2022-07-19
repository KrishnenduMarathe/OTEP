#pragma once
#ifndef TERMINAL_CONTROL_H_
#define TERMINAL_CONTROL_H_

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <X11/keysym.h>

// Singleton Design Pattern for the Control Class
class TerminalControl
{
private:
    static TerminalControl instance;

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
    uint8_t h_lines = 0;

    // Constructor declared private for the pattern to work
    TerminalControl() {}

    /* PRIVATE METHOD DECLARATIONS */
    bool read_config()
    {
        int8_t cnt = 0;

        std::ifstream file;
        std::string line;

        file.open("config/otep.conf", std::ios::in);
        if (this->debug)
        {
            if (file)
            { std::cout << "MSG:: Found File config/otep.conf\n" << std::endl; }
            else 
            { std::cout << "MSG:: Missing Config File" << std::endl; exit(1); }
        }

        std::string var, val;
        bool isValue = false;

        while (!file.eof())
        {
            getline(file, line);

            if (line[0] == '#') { continue; }

            var.assign(""); val.assign("");

            for (uint8_t itr = 0; itr < line.length(); itr++)
            {
                if (line[itr] == '\n' || line[itr] == '\t') { break; }

                if (line[itr] == '=')
                {
                    isValue = true;
                    continue;
                }

                if (isValue)
                {
                    val.push_back(line[itr]);
                } else
                {
                    var.push_back(line[itr]);
                }
            }

            isValue = false;

            // Assign Config Variables
            if (var == "FONT")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val.empty()) { val.assign("fixed"); }
                this->font_name.assign(val);
                cnt++;

            } else if (var == "DARK")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val == "false") { this->dark = false; }
                else { this->dark = true; }
                cnt++;

            } else if (var == "CUSTOM_COLOR")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val.empty()) { val.assign("White"); }
                this->custom_color.assign(val);
                cnt++;

            } else if (var == "FONT_DARK_COLOR")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val.empty()) { val.assign("White"); }
                this->font_dark_color.assign(val);
                cnt++;

            } else if (var == "FONT_CUSTOM_COLOR")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val.empty()) { val.assign("Black"); }
                this->font_custom_color.assign(val);
                cnt++;

            } else if (var == "T_WIDTH")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val != "default")
                {
                    if (val.empty() || !this->isNumber(val) || std::stoi(val) <= 0)
                    { val.assign("default"); }
                }
                this->t_width.assign(val);
                cnt++;

            } else if (var == "T_HEIGHT")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val != "default")
                {
                    if (val.empty() || !this->isNumber(val) || std::stoi(val) <= 0)
                    { val.assign("default"); }
                }
                this->t_height.assign(val);
                cnt++;

            } else if (var == "HISTORY")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val == "true") { this->history = true; }
                else { this->history = false; }
                cnt++;

            } else if (var == "HLINES")
            {
                if (this->debug)
                { std::cout << "CONFIG:: " << var << " := " << val << std::endl; }

                if (val != "default")
                {
                    if (val.empty() || !this->isNumber(val) || std::stoi(val) <= 0)
                    { val.assign("0"); }
                }
                this->h_lines = std::stoi(val);
                cnt++;

            }
        }

        file.close();
        if (cnt == 9) { return false; }
        else { return true; }
    }

    bool isNumber(std::string str)
    {
        for (unsigned int itr = 0; itr < str.length(); itr++)
        {
            if (std::isdigit(str[itr]) == 0) { return false; }
        }

        return true;
    }

public:
    // TEST
    void test()
    { this->debug = true; }
    
    bool exit_loop = false;
    bool debug = false;

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

    // Set copy constructor to Delete
    TerminalControl(const TerminalControl&) = delete;

    static TerminalControl& getInstance() { return instance; }

    /* PUBLIC METHOD DECLARATIONS */
    void initiate();

};


#endif // TERMINAL_CONTROL_H_