#include "control.h"

void process_launch(std::string comm, int* track_h, int* track_w)
{
	// Skip if command empty
	if (comm == "") { return; }

	// Maintain history
	std::ofstream file;
	file.open(".edo_history.txt", std::ios::app);
	file << comm << std::endl;
	file.close();

	std::string msg;
	TerminalControl& terminal = TerminalControl::getInstance();

	// Tokenize input command
	std::vector<std::string> tokens;
	int start = 0;
	for (int i = 0; i < comm.length(); i++)
	{
		if (i != 0 && comm[i] == ' ')
		{
			tokens.push_back(comm.substr(start, i - start));
			start = i+1;
		}
	}
	tokens.push_back(comm.substr(start, comm.length() - start));

	if (tokens[0] == "exit")
	{
		terminal.exit_loop = true;
		return;
	} else {
		msg.assign("EDoShel: " + tokens[0] + " -> Command not found");
	}

	if (*track_w != 0)
	{
		*track_w = 0;
		*track_h += 1;
	}

	if (terminal.charWidth < msg.length())
	{
		for (int w = 0; w < terminal.charWidth; w++)
		{
			terminal.buffer[*track_h][w] = msg[w];
		}
		*track_h += 1;
		*track_w = 0;

		for (int w = terminal.charWidth; w < msg.length(); w++)
		{
			terminal.buffer[*track_h][w - terminal.charWidth] = msg[w];
		}

		*track_w = msg.length() - terminal.charWidth;

	} else {
		for (int w = 0; w < msg.length(); w++)
		{
			terminal.buffer[*track_h][w] = msg[w];
		}

		*track_w = msg.length();
	}

	*track_h += 2;
	*track_w = 0;

	straighten_hw(track_h, track_w);
}