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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>

// Structure to hold data for X Window System
struct {
    int width, height;
    char *text;
    int text_len;

    // X Window Related variables
    Display* display;
    int screen;

    Window root, window;
    GC gc;
    XFontStruct* font;
    unsigned long black_pixel, white_pixel;
}
text_box;

// Connect to Display
static void x_connect()
{
    text_box.display = XOpenDisplay(NULL);
    if (!text_box.display)
    {
        fprintf(stderr, "Could not open Display\n");
        exit(1);
    }

    text_box.screen = DefaultScreen(text_box.display);
    text_box.root = RootWindow(text_box.display, text_box.screen);
    text_box.black_pixel = BlackPixel(text_box.display, text_box.screen);
    text_box.white_pixel = WhitePixel(text_box.display, text_box.screen);
}

// Create a new window
static void create_window()
{
    text_box.width = 300;
    text_box.height = 300;

    int x = 0;
    int y = 0;
    int border = 0;

    text_box.window = XCreateSimpleWindow(text_box.display, text_box.root, x, y, text_box.width, text_box.height, border, text_box.black_pixel, text_box.white_pixel);
    XSelectInput(text_box.display, text_box.window, ExposureMask);
    XMapWindow(text_box.display, text_box.window);
}

// Set up Graphical Context
static void set_up_gc()
{
    text_box.gc = XCreateGC(text_box.display, text_box.window, 0, 0);
    XSetBackground(text_box.display, text_box.gc, text_box.white_pixel);
    XSetForeground(text_box.display, text_box.gc, text_box.black_pixel);
}

// Set up Text Font
static void set_up_font()
{
    const char* fontname = "-misc-fixed-bold-r-normal--13-100-100-100-c-70-iso8859-1";
    text_box.font = XLoadQueryFont(text_box.display, fontname);
    // Backup if above fails
    if (!text_box.font)
    {
        fprintf(stderr, "Setting the backup font\n");
        text_box.font = XLoadQueryFont(text_box.display, "fixed");
    }
    XSetFont(text_box.display, text_box.gc, text_box.font->fid);
}

// Draw routine
static void draw_screen()
{
    int x, y, direction, ascent, descent;
    XCharStruct overall;

    XTextExtents(text_box.font, text_box.text, text_box.text_len, &direction, &ascent, &descent, &overall);
    // Set text to the center of the screen
    x = (text_box.width - overall.width) / 2;
    y = text_box.height / 2 + (ascent -descent) / 2;
    XClearWindow(text_box.display, text_box.window);
    XDrawString(text_box.display, text_box.window, text_box.gc, x, y, text_box.text, text_box.text_len);
}

// Event Loop
static void event_loop()
{
    while (1)
    {
        XEvent e;
        XNextEvent(text_box.display, &e);
        if (e.type == Expose)
        {
            draw_screen();
        }
    }
}

// Main Driver
int main()
{
    text_box.text = "Hello World!";
    text_box.text_len = strlen(text_box.text);

    x_connect();
    create_window();
    set_up_gc();
    set_up_font();
    event_loop();

    return 0;
}
