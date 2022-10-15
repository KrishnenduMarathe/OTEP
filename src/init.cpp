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

	// Get linux PATH environment variable
	//char* path = std::getenv("PATH");
	//std::cout << path << std::endl;

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

bool TerminalControl::read_config()
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
        if (var == "DEBUG")
        {
            if (val == "true") { this->debug = true; }
            else { this->debug = false; }

            if (this->debug)
            { std::cout << "CONFIG:: DEBUG Enabled" << std::endl; }
            
        } else if (var == "ADVANCE")
		{
			if (val == "true") { this->advance_debug = true; }
			else { this->advance_debug = false; }

			if (!this->debug) { this->advance_debug = false; }

			if (this->advance_debug)
            { std::cout << "CONFIG:: ADVANCE DEBUG Enabled" << std::endl; }

		} else if (var == "FONT")
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
	std::cout << std::endl;
		
    if (cnt == 9) { return false; }
    else { return true; }
}

void TerminalControl::terminate()
{
    if (this->debug)
    {
        std::cout << "\nMSG:: Closing Application\n";
    }

    // Free Buffer Data
    for (int h = 0; h < this->charHeight; h++)
    {
        delete[] this->buffer[h];
    }
    delete[] this->buffer;

    XDestroyWindow(this->display, this->main);
    XCloseDisplay(this->display);

    if (this->debug)
    {
        std::cout << "MSG:: Application Closed Successfully\n";
    }
}