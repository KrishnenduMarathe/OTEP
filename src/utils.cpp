#include "control.h"

void display_resize()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Free Buffer Data
    for (int h = 0; h < terminal.charHeight; h++)
    {
        delete[] terminal.buffer[h];
    }
    delete[] terminal.buffer;

    // Calculate Character dimensions
    terminal.charWidth = static_cast<int>(terminal.width / terminal.c_width);
    terminal.charHeight = static_cast<int>(terminal.height / terminal.c_height);

    /// Default Values
    terminal.buffer = new char*[terminal.charHeight];
    for (int h = 0; h < terminal.charHeight; h++)
    {
        terminal.buffer[h] = new char[terminal.charWidth];
        for (int w = 0; w < terminal.charWidth; w++)
        {
            terminal.buffer[h][w] = ' ';
        }
    }

    if (!terminal.edo)
    {
        std::string noShell = "EDoShell Not Found!";
        for (int w = 0; w < terminal.charWidth; w++)
        {
            if (w == noShell.length()) { break; }
            terminal.buffer[0][w] = noShell[w];
        }
    } else
    {
        // Update Buffer Data Routine & run shell
    }
}

// Draw Rows of the terminal
void draw_width(int current_height)
{
    TerminalControl& terminal = TerminalControl::getInstance();

    int x, y ,direction, ascent, descent;
    XCharStruct all;
    XColor color, trueColor;

    // Left Top Corner Coordinates
    int lx, ly;

    lx = 0;
    ly = current_height * terminal.c_height;

    x = lx;
    y = ly + terminal.c_height;

    // Draw Background
    XSetForeground(terminal.display, terminal.gc, terminal.bg);
    for (int h = y - terminal.c_height; h < y; h++)
    {
        for (int w = x; w < terminal.width; w++)
        {
            XDrawPoint(terminal.display, terminal.main, terminal.gc, w, h);
        }
    }

    // Draw String
    XSetForeground(terminal.display, terminal.gc, terminal.fg);
    XDrawString(terminal.display, terminal.main, terminal.gc, x, y, terminal.buffer[current_height], terminal.charWidth);
}

// Draw Function
void draw()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Single thread draw
    for (int h = 0; h <terminal.charHeight; h++)
    {
        draw_width(h);
    }

}