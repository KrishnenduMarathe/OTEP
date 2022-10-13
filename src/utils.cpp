#include "control.h"
#include <X11/Xlib.h>

void display_resize()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Free Buffer Data
	char **record;
	if (terminal.charHeight != 0)
	{
		record = new char*[terminal.charHeight];
	}
	for (int h = 0; h < terminal.charHeight; h++)
    {
		record[h] = new char[terminal.charWidth];
		for (int w = 0; w < terminal.charWidth; w++)
		{
			record[h][w] = terminal.buffer[h][w];
		}
        delete[] terminal.buffer[h];
    }
    delete[] terminal.buffer;

	int old_x = terminal.charWidth;
	int old_y = terminal.charHeight;

    // Calculate Character dimensions
    terminal.charWidth = static_cast<int>(terminal.width / terminal.c_width);
    terminal.charHeight = static_cast<int>(terminal.height / terminal.c_height);

    // Default Values
    terminal.buffer = new char*[terminal.charHeight];
	for (int h = 0; h < terminal.charHeight; h++)
    {
        terminal.buffer[h] = new char[terminal.charWidth];
        for (int w = 0; w < terminal.charWidth; w++)
        {
            terminal.buffer[h][w] = ' ';
        }
    }

	// Restore buffer
	if (old_x != 0 && old_y != 0)
	{
		int h = terminal.charHeight - 1;
		int hi = old_y - 1;
		int w = 0;
		while (h >= 0)
		{
			terminal.buffer[h][w%terminal.charWidth] = record[hi][w%old_x];

			if (w != 0 && w % terminal.charWidth == 0)
			{ h--; }
			if (w != 0 && w % old_x == 0)
			{ hi--; }
			w++;
		}
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

    // Draw String
    XSetForeground(terminal.display, terminal.gc, terminal.fg);
    XDrawString(terminal.display, terminal.main, terminal.gc, x, y, terminal.buffer[current_height], terminal.charWidth);
}

// Draw Function
void draw()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Clear Screen
    XClearWindow(terminal.display, terminal.main);

	// Draw Prompt

    // Single thread draw
    for (int h = 0; h < terminal.charHeight; (h++))
    {
        draw_width(h);
    }

}