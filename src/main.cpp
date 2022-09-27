#include "control.h"

// Initialise Static Variable
TerminalControl TerminalControl::instance;

int main(int argc, char** argv)
{
	TerminalControl& handle = TerminalControl::getInstance();
	
	handle.initiate();
    event_loop();
    handle.terminate();

    return 0;
}