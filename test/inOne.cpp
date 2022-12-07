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

#include <iostream>
#include <string>
#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

struct
{
    int width, height;
    char config;
    std::string text;
    int text_len;

    Display* display;
    int screen;
    Window root;
    Window mainWindow;
    GC gc;
    XFontStruct* font;

    unsigned long white_pixel, black_pixel;
    unsigned int display_width, display_height;

    Colormap colorMap;
    int c_height, c_width;
    XEvent events;
}
terminal;

#include "kb_common.h"

void initiate_terminal()
{
    terminal.display = XOpenDisplay(nullptr);
    if (!terminal.display)
    {
        std::cout << "Error: Could not open Display!\n";
        exit(1);
    }
    terminal.screen = DefaultScreen(terminal.display);
    terminal.root = RootWindow(terminal.display, terminal.screen);
    terminal.black_pixel = BlackPixel(terminal.display, terminal.screen);
    terminal.white_pixel =- WhitePixel(terminal.display, terminal.screen);
    terminal.display_width = DisplayWidth(terminal.display, terminal.screen);
    terminal.display_height = DisplayHeight(terminal.display, terminal.screen);
    terminal.width = terminal.display_width / 3;
    terminal.height = terminal.display_height / 3;

    // Create Window
    terminal.mainWindow = XCreateSimpleWindow(terminal.display, terminal.root, 0, 0, terminal.width, terminal.height, 0, terminal.white_pixel, terminal.black_pixel);
    XSelectInput(terminal.display, terminal.mainWindow, ExposureMask | KeyReleaseMask | KeyPressMask | StructureNotifyMask | ButtonPressMask);
    XMapWindow(terminal.display, terminal.mainWindow);
    XFlush(terminal.display);

    terminal.colorMap = DefaultColormap(terminal.display, terminal.screen);

    // GC
    XGCValues values;
    terminal.gc = XCreateGC(terminal.display, terminal.mainWindow, (GCForeground | GCBackground), &values);
    XSetBackground(terminal.display, terminal.gc, terminal.black_pixel);
    XSetForeground(terminal.display, terminal.gc, terminal.white_pixel);

    // Font
    std::string fontName = "-misc-fixed-bold-r-normal--13-100-100-100-c-70-iso8859-1";
    terminal.font = XLoadQueryFont(terminal.display, fontName.c_str());
    if (!terminal.font)
    {
        std::cout << "Cannot use given input font. Changing to defualt!\n";
        XLoadQueryFont(terminal.display, "fixed");
    }
    XSetFont(terminal.display, terminal.gc, terminal.font->fid);

    // Will be used in the main program
    XCharStruct dummy;
    int direction, ascent, descent;
    std::string dumb = "Tt"; // It Looks Like PI!!
    XTextExtents(terminal.font, dumb.c_str(), 1, &direction, &ascent, &descent, &dummy);
    // Storing character dimensions of the font in pixel
    terminal.c_width = dummy.width;
    terminal.c_height = dummy.ascent - dummy.descent;

    // Window Title
    XStoreName(terminal.display, terminal.mainWindow, "inOne");
    XSync(terminal.display, False);
    XClearWindow(terminal.display, terminal.mainWindow);
}

void draw(int flg)
{
    XClearWindow(terminal.display, terminal.mainWindow);

    int x, y, direction, ascent, descent;
    XCharStruct overall;
    XColor color, trueColor;

    // Top Left Corner position
    int lx, ly;
    lx = terminal.width / 2;
    ly = terminal.height / 2;

    XTextExtents(terminal.font, terminal.text.c_str(), terminal.text_len, &direction, &ascent, &descent, &overall);

    /*
    overall.width = total width of the text drawn
    x = lx
    y = bottom coordinate of string on Y axis
    y = ly + (ascent - descent)
    */

   x = lx;
   y = ly + (ascent - descent);

   // Centering text
   x -= overall.width / 2;
   y -= (ascent - descent) / 2;

   // Draw Background First
   switch (flg)
   {
        case 0:
            XAllocNamedColor(terminal.display, terminal.colorMap, "Purple", &color, &trueColor);
            break;
        
        case 1:
            XAllocNamedColor(terminal.display, terminal.colorMap, "Green", &color, &trueColor);
            break;
        
        default:
            XAllocNamedColor(terminal.display, terminal.colorMap, "Gray", &color, &trueColor);
            break;
   }

   color.flags = DoRed | DoGreen | DoBlue;
   XSetForeground(terminal.display, terminal.gc, color.pixel);

   for (int h = y - 1 - (overall.ascent - overall.descent); h <= y; h++)
   {
        for (int w = x; w < x+overall.width; w++)
        {
            XDrawPoint(terminal.display, terminal.mainWindow, terminal.gc, w, h);
        }
   }

   // Now Draw String
   XAllocNamedColor(terminal.display, terminal.colorMap, "White", &color, &trueColor);
   color.flags = DoRed | DoGreen | DoBlue;
   XSetForeground(terminal.display, terminal.gc, color.pixel);
   XDrawString(terminal.display, terminal.mainWindow, terminal.gc, x, y, terminal.text.c_str(), terminal.text_len);
}

void event_loop()
{
    // Define Windows close event when x is pressed on the title bar
    Atom windowDelete = XInternAtom(terminal.display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(terminal.display, terminal.mainWindow, &windowDelete, 1);

    int flag = 0;
    while (true)
    {
        XNextEvent(terminal.display, &terminal.events);
        if (terminal.events.type == Expose)
        {
            XClearWindow(terminal.display, terminal.mainWindow);
            draw(flag);
        } else if (terminal.events.type == ConfigureNotify)
        {
            XConfigureEvent xce = terminal.events.xconfigure;
            if (xce.width != terminal.width || xce.height != terminal.height)
            {
                terminal.height = xce.height;
                terminal.width = xce.width;
                draw(flag);
            }
        } else if (terminal.events.type  == KeyPress)
        {
            if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Shift_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Shift_R)
            {
                kb_triggers.shift = true;
            } else if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Control_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Control_R)
            {
                kb_triggers.control = true;
            } else if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Alt_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Alt_R)
            {
                kb_triggers.alt = true;
            } else if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Caps_Lock)
            {
                kb_triggers.caps= !kb_triggers.caps;
            }

            // DEBUG
            std::cout << "\n>> CAPS: " << kb_triggers.caps << " >> Shift: " << kb_triggers.shift << " >> Control: " << kb_triggers.control << " >> Alt: " << kb_triggers.alt << std::endl << std::endl;

            // DEBUG
            char tmp = what_is_char();
            std::cout << "Key Pressed: " << tmp << std::endl;
            // Spacebar
            if (tmp == ' ')
            {
                flag++;
                flag = flag % 3;
                draw(flag);
            }
        } else if (terminal.events.type == KeyRelease)
        {
            if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Shift_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Shift_R)
            {
                kb_triggers.shift = false;
            } else if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Control_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Control_R)
            {
                kb_triggers.control = false;
            } else if (XLookupKeysym(&terminal.events.xkey, 0) == XK_Alt_L || XLookupKeysym(&terminal.events.xkey, 0) == XK_Alt_R)
            {
                kb_triggers.alt = false;
            }

            // DEBUG
            std::cout << "\n>> CAPS: " << kb_triggers.caps << " >> Shift: " << kb_triggers.shift << " >> Control: " << kb_triggers.control << " >> Alt: " << kb_triggers.alt << std::endl << std::endl;
        
        } else if (terminal.events.type == ButtonPress)
        {
            if (terminal.events.xbutton.button == Button4)
            {
                // DEBUG
                std::cout << "MWHEEL: up\n";
            } else if (terminal.events.type == Button5)
            {
                // DEBUG
                std::cout << "MWHEEL: down\n";
            }
        } else if (terminal.events.type == ClientMessage)
        { break; }

        std::flush(std::cout); // flush debug messages
        XFlush(terminal.display); // flush display
    }
}

int main()
{
    terminal.text.assign("My name is OTEP - inOne. A test suite of necessary features.");
    terminal.text_len = terminal.text.length();
    
    initiate_terminal();
    event_loop();
    
    XDestroyWindow(terminal.display, terminal.mainWindow);
    XCloseDisplay(terminal.display);
    return 0;
}
