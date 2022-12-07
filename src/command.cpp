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

void process_launch(std::string comm, int* track_h, int* track_w)
{
	// Skip if command empty
	if (comm == "") { return;  }
	TerminalControl& terminal = TerminalControl::getInstance();

	// Maintain history
	std::ofstream file;
	file.open(".edo_history.txt", std::ios::app);

	// Tokenize input command
	int start, end;
	bool comm_found = false;

	for (int i = 0; i < comm.length(); i++)
	{
		if (!comm_found)
		{
			if (comm[i] != ' ')
			{
				start = i;
				comm_found = true;
			}
		} else {
			if (comm[i] == ' ' || i == comm.length())
			{
				end = i;
				break;
			}
		}
	}

	file << comm.substr(start, comm.length() - start) << std::endl;
	file.close();

	std::string msg;
	if (comm.substr(start, end - start) == "exit")
	{
		terminal.exit_loop = true;
		return;
	} else if (comm.substr(start, end - start) == "clear")
	{
		*track_h = 0;
		*track_w = 0;
		init_display();
		terminal.draw_prompt = true;
		return;
	} else {
		// Look if command binary available
		int loc = -1;

		for (int v = 0; v < terminal.path.size(); v++)
		{
			std::ifstream bin;
			bin.open(terminal.path[v]+"/"+comm.substr(start, end - start), std::ios::in);

			// file exists
			if (bin) { loc = v; break; }
			
			bin.close();
		}

		if (loc >= 0)
		{
			// Execute command
			char buffer[128];
			FILE* process = popen((terminal.path[loc]+"/"+comm.substr(start, comm.length() - start)).c_str(), "r");

			if (!process)
			{
				msg.assign("EDoShell: "+comm.substr(start, end - start)+" Failed to launch command");
			} else {
				// Read process output
				msg.assign("");
				while (!feof(process))
				{
					// read buffer
					if (fgets(buffer, 128, process) != NULL)
					{
						msg += buffer;
					}
				}
			}
			pclose(process);
		} else {
			msg.assign("EDoShell: "+comm.substr(start, end - start)+" -> Command not found");
		}
	}

	if (*track_w != 0)
	{
		*track_w = 0;
		*track_h += 1;
	}

	if (terminal.charWidth <= msg.length())
	{
		for (int w = 0; w < msg.length(); w++)
		{
			if (w != 0 && w%terminal.charWidth == 0)
			{
				*track_h += 1;
				*track_w = 0;
				straighten_hw(track_h, track_w);
			}

			// handle escape characters: \n \t
			if (msg[w] == '\n')
			{
				*track_h += 1;
				*track_w = 0;
				straighten_hw(track_h, track_w);
				continue;
			} else if (msg[w] == '\t')
			{
				for (int i = 0; i < 2; i++)
				{
					terminal.buffer[(*track_h*terminal.charWidth)+*track_w] = ' ';

					*track_w += 1;
					straighten_hw(track_h, track_w);
				}
				continue;
			}

			terminal.buffer[(*track_h*terminal.charWidth)+*track_w] = msg[w];
			*track_w += 1;
			straighten_hw(track_h, track_w);
		}
	} else {
		for (int w = 0; w < msg.length(); w++)
		{
			// handle escape characters: \n \t
			if (msg[w] == '\n')
			{
				*track_h += 1;
				*track_w = 0;
				straighten_hw(track_h, track_w);
				continue;
			} else if (msg[w] == '\t')
			{
				for (int i = 0; i < 2; i++)
				{
					terminal.buffer[(*track_h*terminal.charWidth)+*track_w] = ' ';

					*track_w += 1;
					straighten_hw(track_h, track_w);
				}
				continue;
			}

			terminal.buffer[(*track_h*terminal.charWidth)+*track_w] = msg[w];
			*track_w += 1;
			straighten_hw(track_h, track_w);
		}
	}
	
	*track_h += 2;
	*track_w = 0;
	straighten_hw(track_h, track_w);
}
