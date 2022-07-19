#include "control.h"

void TerminalControl::initiate()
{
    if (this->init)
    {
        if (this->debug)
        { std::cout << "\nError: Init failed!" << std::endl; }
        return;
    }
    this->init = true;

    if (this->read_config())
    {
        if (this->debug)
        { std::cout << "\nError: Incomplete/Corrupted config file" << std::endl; }
        exit(1);
    }

    // Connect to X Server
    this->display = XOpenDisplay(NULL);
    if (!this->display)
    {
        if (this->debug)
        { std::cout << "\nError: Couldn't connect to X Server" << std::endl; }
        exit(1);
    }

    this->screen = DefaultScreen(this->display);
    this->root = RootWindow(this->display, this->screen);
    this->blackpx = BlackPixel(this->display, this->screen);
    this->whitepx = WhitePixel(this->display, this->screen);

    this->resolution_x = DisplayWidth(this->display, this->screen);
    this->resolution_y = DisplayHeight(this->display, this->screen);

    // Set Window Size from Config Variables
    if (this->t_width == "default")
    { this->width = this->resolution_x / 3; }
    else
    { this->width = std::stoi(this->t_width); }

    if (this->t_height == "default")
    { this->height = this->resolution_y / 3; }
    else
    { this->height = std::stoi(this->t_height); }

    XGCValues values;
    this->cmap = DefaultColormap(this->display, this->screen);

    // Creating Terminal Window
    if (this->dark)
    {
        this->main = XCreateSimpleWindow(this->display, this->root, 0, 0, this->width, this->height, 0, this->whitepx, this->blackpx);
    } else
    {
        if (XAllocNamedColor(this->display, this->cmap, this->custom_color.c_str(), &this->u_color, &this->s_color) == 0)
        {
            this->main = XCreateSimpleWindow(this->display, this->root, 0, 0, this->width, this->height, 0, this->blackpx, this->whitepx);
            values.background = this->whitepx;
        }
    }
}