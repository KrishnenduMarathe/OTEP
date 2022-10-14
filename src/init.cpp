#include "control.h"

void TerminalControl::initiate()
{
    if (this->init)
    {
        std::cout << "\nERROR:: Init failed!" << std::endl;
        return;
    }
    this->init = true;

    if (this->read_config())
    {
        if (this->debug)
        { std::cout << "\nERROR:: Incomplete/Corrupted config file" << std::endl; }
        exit(1);
    }

	// Get Hostname (Linux System)
	std::ifstream file;
	std::string hostname;
	file.open("/etc/hostname", std::ios::in);
	getline(file, hostname);
	file.close();

	this->prompt.assign(":: " + hostname + " >> ");

    // Connect to X Server
    this->display = XOpenDisplay(NULL);
    if (!this->display)
    {
        if (this->debug)
        { std::cout << "\nERROR:: Couldn't connect to X Server" << std::endl; }
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
        } else
        {
            this->u_color.flags = DoRed | DoGreen | DoBlue;
            values.background = this->u_color.pixel;
            this->main = XCreateSimpleWindow(this->display, this->root, 0, 0, this->width, this->height, 0, this->blackpx, this->u_color.pixel);
        }
    }

    XSelectInput(this->display, this->main, ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | StructureNotifyMask);
    XMapWindow(this->display, this->main);
    XFlush(this->display);

    // Graphical Context
    if (this->dark)
    {
        values.background = this->blackpx;
        if (XAllocNamedColor(this->display, this->cmap, this->font_dark_color.c_str(), &this->u_color, &this->s_color) == 0)
        {
            values.foreground = this->whitepx;
        } else
        {
            this->u_color.flags = DoRed | DoGreen | DoBlue;
            values.foreground = this->u_color.pixel;
        }
    } else
    {
        if (XAllocNamedColor(this->display, this->cmap, this->font_custom_color.c_str(), &this->u_color, &this->s_color) == 0)
        {
            values.foreground = this->blackpx;
        } else
        {
            this->u_color.flags = DoRed | DoGreen | DoBlue;
            values.foreground = this->u_color.pixel;
        }
    }

    this->gc = XCreateGC(this->display, this->main, (GCForeground | GCBackground), &values);
    this->bg = values.background;
    this->fg = values.foreground;

    XSetBackground(this->display, this->gc, values.background);
    XSetForeground(this->display, this->gc, values.foreground);

    // Font
    // Set FONT variable in the config file from the list of the fonts installed on your system
    // use "xlsfonts" command to get a list of these fonts
    this->font = XLoadQueryFont(this->display, this->font_name.c_str());
    if (!this->font)
    {
        if (this->debug)
        { std::cout << "WARNING:: Unable to load font \"" << this->font_name << "\"\n\t\tFalling to default font" << std::endl; }
        this->font = XLoadQueryFont(this->display, "fixed");
    }
    XSetFont(this->display, this->gc, this->font->fid);

    // Set Character dimensions
    XCharStruct dummy;
    int direct, ascnt, dscnt;
    std::string dumb = "ABC";
    XTextExtents(this->font, dumb.c_str(), 1, &direct, &ascnt, &dscnt, &dummy);

    this->c_width = dummy.width;
    this->c_height = dummy.ascent - dummy.descent;

    // Set Window Title
    XStoreName(this->display, this->main, "OTEP");

    XSync(this->display, False);
    XClearWindow(this->display, this->main);
    
    // Initialize Frame Buffer
    display_resize();
}