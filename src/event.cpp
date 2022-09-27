#include "keyboard.h"

void event_loop()
{
    TerminalControl& terminal = TerminalControl::getInstance();
    Atom windowCross = XInternAtom(terminal.display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(terminal.display, terminal.main, &windowCross, 1);

	int track_h = 0;
	int track_w = 0;

    while (!terminal.exit_loop)
    {
        XNextEvent(terminal.display, &terminal.event);
        if (terminal.event.type == Expose)
        {
            XClearWindow(terminal.display, terminal.main);
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
                    
                    XClearWindow(terminal.display, terminal.main);
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
                char key = terminal.getKeyInChar(); // Add backspace support
                if (terminal.debug)
                { std::cout << "MSG:: Key Pressed >> " << key << std::endl; }
                        
                // Process Input
				if (key == '\n')
				{
					for (int w = track_w; w < terminal.charWidth; w++)
					{
						terminal.buffer[track_h][w] = ' ';
					}
					track_h++;
					track_w = 0;
				} else
				{
					terminal.buffer[track_h][track_w] = key;
					track_w++;
				}
				if (track_w >= terminal.charWidth)
				{
					track_w = 0; 
					track_h++;
				}
				if (track_h >= terminal.charHeight - 1)
				{
					track_h--;
					for (int h = 1; h < terminal.charHeight; h++)
					{
						for (int w = 0; w < terminal.charWidth; w++)
						{
							terminal.buffer[h-1][w] = terminal.buffer[h][w];
						}
					}
					for (int h = terminal.charHeight - 1 - 1; h < terminal.charHeight; h++)
					{
						for (int w = 0; w < terminal.charWidth; w++)
						{
							terminal.buffer[h][w] = ' ';
						} 
					}
				}
            }
            // Update Screen if required & draw

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
            // Update Screen if required & draw

        } else if (terminal.event.type == ButtonPress)
        {
            // Detect Scroll
            if (terminal.event.xbutton.button == Button4)
            {
                // Process Scroll
                if (terminal.debug)
                { std::cout << "MSG:: Mouse Scroll Up\n"; }
            } else if (terminal.event.xbutton.button == Button5)
            {
                // Process Scroll
                if (terminal.debug)
                { std::cout << "MSG:: Mouse Scroll Down\n"; }
            }

        } else if (terminal.event.type == ClientMessage)
        { terminal.exit_loop = true; }

		draw();

        std::flush(std::cout);
        XFlush(terminal.display);
    }
}