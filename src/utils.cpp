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

#include "control.h"

bool TerminalControl::isNumber(std::string str)
{
	for (unsigned int itr = 0; itr < str.length(); itr++)
    {
        if (std::isdigit(str[itr]) == 0) { return false; }
    }

    return true;
}

// Resize display buffer
void display_resize()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Free Buffer Data
	char *record;
	record = new char[terminal.charHeight * terminal.charWidth];
	for (int xy = 0; xy < terminal.charHeight * terminal.charWidth; xy++)
    {
		record[xy] = terminal.buffer[xy];
    }
    delete[] terminal.buffer;

	int old_x = terminal.charWidth;
	int old_y = terminal.charHeight;

    // Calculate Character dimensions
    terminal.charWidth = static_cast<int>(terminal.width / terminal.c_width);
    terminal.charHeight = static_cast<int>(terminal.height / terminal.c_height);

    // Allocate Buffer
    terminal.buffer = new char[terminal.charHeight * terminal.charWidth];

	// Restore buffer
	if (old_x != 0 && old_y != 0)
	{
		int old_xy = old_x * old_y;
		int new_xy = terminal.charHeight * terminal.charWidth;

		if (old_xy >= new_xy)
		{
			for (int xy = new_xy - 1; xy >= 0; xy--)
			{
				terminal.buffer[xy] = record[xy];
			}
		} else {
			for (int xy = 0; xy < new_xy; xy++)
			{
				if (xy < old_xy)
				{
					terminal.buffer[xy] = record[xy];
				} else {
					terminal.buffer[xy] = ' ';
				}
			}
		}
	}

	// Clear record
	delete[] record;
}

// Fill buffer with blanks
void init_display()
{
	TerminalControl& terminal = TerminalControl::getInstance();

	for (int xy = 0; xy < terminal.charHeight * terminal.charWidth; xy++)
	{
		terminal.buffer[xy] = ' ';
	}
}

// Draw Rows of the terminal
void draw_width(int current_height)
{
    TerminalControl& terminal = TerminalControl::getInstance();

    int x, y;

    // Left Top Corner Coordinates
    int lx, ly;

    lx = 0;
    ly = current_height * terminal.c_height;

    x = lx;
    y = ly + terminal.c_height;

	// Generate string
	char *row = new char[terminal.charWidth];
	for (int w = 0; w < terminal.charWidth; w++)
	{
		row[w] = terminal.buffer[(current_height*terminal.charWidth)+w];
	}

    // Draw String
    XSetForeground(terminal.display, terminal.gc, terminal.fg);
    XDrawString(terminal.display, terminal.main, terminal.gc, x, y, row, terminal.charWidth);

	// free generated string
	delete[] row;
}

// Draw Function
void draw()
{
    TerminalControl& terminal = TerminalControl::getInstance();

    // Clear Screen
    XClearWindow(terminal.display, terminal.main);

    // Single thread draw
    for (int h = 0; h < terminal.charHeight; h++)
    {
        draw_width(h);
    }

}

// Manage pointer to the current buffer addresss within constraints
void straighten_hw(int* track_h, int* track_w)
{
	TerminalControl& terminal = TerminalControl::getInstance();

	if (*track_w >= terminal.charWidth)
	{
		*track_w = 0;
		*track_h += 1;
	}

	if (*track_h >= terminal.charHeight - 1)
	{
		*track_h -= 1;

		for (int xy = 0; xy < terminal.charHeight * terminal.charWidth; xy++)
		{
			if (xy < (terminal.charHeight - 1)*terminal.charWidth)
			{
				terminal.buffer[xy] = terminal.buffer[xy + terminal.charWidth];
			} else {
				terminal.buffer[xy] = ' ';
			}
		}
	}
}
