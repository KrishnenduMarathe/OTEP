#include "control.h"

// Initialise Static Variable
TerminalControl TerminalControl::instance;

int main(int argc, char** argv)
{
    
    TerminalControl::getInstance().initiate();
    event_loop();
    TerminalControl::getInstance().terminate();

    return 0;
}