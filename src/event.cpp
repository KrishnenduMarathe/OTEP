#include "keyboard.h"

void event_loop()
{
    TerminalControl& terminal = TerminalControl::getInstance();
    Atom windowCross = XInternAtom(terminal.display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(terminal.display, terminal.main, &windowCross, 1);

	// Input Pointer Coordinates
	int track_h = 0;
	int track_w = 0;

	// Command Initial Coordinates
	int comm_h = 0;
	int comm_w = 0;

	std::string msg;
	std::string command;

	if (terminal.debug && !terminal.advance_debug)
	{
		std::cout << "Input Pointer >>\n\t";
	}

    while (!terminal.exit_loop)
    {
		// Draw prompt
		if (terminal.draw_prompt)
		{
			terminal.draw_prompt = false;

			if (track_w != 0)
			{
				track_w = 0;
				track_h++;
			}

			if (terminal.charWidth <= terminal.prompt.length())
			{
				for (int w = 0; w < terminal.prompt.length(); w++)
				{
					if (w == terminal.charWidth) { track_h++; }

					terminal.buffer[(track_h*terminal.charWidth)+(w%terminal.charWidth)] = terminal.prompt[w];
				}

				track_w = terminal.prompt.length() - terminal.charWidth;

			} else {
				for (int w = 0; w < terminal.prompt.length(); w++)
				{
					terminal.buffer[(track_h*terminal.charWidth)+w] = terminal.prompt[w];
				}

				track_w = terminal.prompt.length();
			}

			straighten_hw(&track_h, &track_w);

			comm_h = track_h;
			comm_w = track_w;
		}
			
        XNextEvent(terminal.display, &terminal.event);
        if (terminal.event.type == Expose)
        {
            display_resize();

        } else if(terminal.event.type == ConfigureNotify)
        {
            XConfigureEvent xce = terminal.event.xconfigure;
            if (xce.width != terminal.width || xce.height != terminal.height)
            {
                XConfigureEvent xce = terminal.event.xconfigure;
                		
				if (xce.width != terminal.width || xce.height != terminal.height)
                {
                    terminal.height = xce.height;
                    terminal.width = xce.width;
					display_resize();
                }
            }
        }  else if(terminal.event.type == KeyPress)
        {
            if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Shift_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Shift_R)
            {
                terminal.kb_triggers.shift = true;
            } else if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Control_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Control_R)
            {
                terminal.kb_triggers.control = true;
            } else if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Alt_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Alt_R)
            {
                terminal.kb_triggers.alt = true;
            } else if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Caps_Lock)
            {
                terminal.kb_triggers.caps = !terminal.kb_triggers.caps;
            } else
            {
                char key = terminal.getKeyInChar();
				
                if (terminal.advance_debug)
                { std::cout << "MSG:: Key Pressed >> " << std::hex << (int) key << " { " << key << " }" << std::endl; }

                // Process Input
				if (key == '\n')
				{
					// construct command
					 command.assign("");

					for (int h = comm_h; h < track_h+1; h++)
					{
						for (int w = comm_w; w < track_w; w++)
						{
							command.push_back(terminal.buffer[(h*terminal.charWidth)+w]);
						}
					}

					track_h += 1;
					track_w = 0;

					// process command
					process_launch(command, &track_h, &track_w);

					if (!terminal.exit_loop) { terminal.draw_prompt = true; }
				} else if (key == '\b')
				{
					if (track_w > comm_w)
					{
						track_w--;
						if (track_w < 0)
						{
							track_w = terminal.charWidth - 1;
							track_h--;
						}

						terminal.buffer[(track_h*terminal.charWidth)+track_w] = ' ';
					}
				} else
				{
					if (key == '\0') { key = ' '; }

					terminal.buffer[(track_h*terminal.charWidth)+track_w] = key;
					track_w++;
				}

				straighten_hw(&track_h, &track_w);
            }
            // Update Screen if required

        } else if(terminal.event.type == KeyRelease)
        {
            if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Shift_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Shift_R)
            {
                terminal.kb_triggers.shift = false;
            } else if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Control_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Control_R)
            {
                terminal.kb_triggers.control = false;
            } else if (XLookupKeysym(&terminal.event.xkey, 0) == XK_Alt_L || XLookupKeysym(&terminal.event.xkey, 0) == XK_Alt_R)
            {
                terminal.kb_triggers.alt = false;
            }
            // Update Screen if required

        } else if (terminal.event.type == ButtonPress)
        {
            // Detect Scroll
            if (terminal.event.xbutton.button == Button4)
            {
                // Process Scroll
                if (terminal.advance_debug)
                { std::cout << "MSG:: Mouse Scroll Up\n"; }
            } else if (terminal.event.xbutton.button == Button5)
            {
                // Process Scroll
                if (terminal.advance_debug)
                { std::cout << "MSG:: Mouse Scroll Down\n"; }
            }

        } else if (terminal.event.type == ClientMessage)
        { terminal.exit_loop = true; }

		draw();

        std::flush(std::cout);
        XFlush(terminal.display);

		if (terminal.debug && !terminal.advance_debug)
		{
			msg.assign("H: " + std::to_string(track_h) + " | W: " + std::to_string(track_w));
			std::cout << msg;
			for (int i = 0; i < msg.length(); i++)
			{
				std::cout << "\b";
			}
		}
    }

	if (terminal.debug && !terminal.advance_debug) { std::cout << std::endl; }
}