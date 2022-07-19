// Change Window color on command

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

int main()
{
    // Let's go primitive
    Display* display;
    int screen_num;
    Window win;
    Window root;
    Colormap cmap;
    unsigned int display_width, display_height;
    unsigned int width, height;
    GC gc;
    XFontStruct* font;

    display = XOpenDisplay(getenv("DISPLAY"));
    if (display == NULL)
    {
        fprintf(stderr, "Could not open Display\n");
        exit(1);
    }

    screen_num = DefaultScreen(display);;
    display_width = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);

    width = display_width/3;
    height = display_height/3;

    root = RootWindow(display, screen_num);
    unsigned long white_pixel, black_pixel;
    black_pixel = BlackPixel(display, screen_num);
    white_pixel = WhitePixel(display, screen_num);

    int win_border_width = 0;
    
    win = XCreateSimpleWindow(display, root, 0, 0, width, height, win_border_width, white_pixel, black_pixel);
    XSelectInput(display, win, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(display, win);
    XFlush(display);
    
    cmap = DefaultColormap(display, screen_num);
    XColor color, trueColor;
    XGCValues values;

    gc = XCreateGC(display, win, (GCForeground | GCBackground), &values);
    XSetForeground(display, gc, white_pixel);
    XSetBackground(display, gc, black_pixel);
    XSync(display, False);
    XClearWindow(display, win);

    // Event Handling
    XEvent e;
    int flag = 0;
    int draw = 0;
    while (1)
    {
        XNextEvent(display, &e);
        if (e.type == Expose)
        {
            draw = 1;
        } else if (e.type == ConfigureNotify)
        {
            XConfigureEvent xce = e.xconfigure;
            if (xce.width != width || xce.height != height)
            {
                height = xce.height;
                width = xce.width;
            }
            draw = 1;
        } else if (e.type == KeyPress)
        {
            fprintf(stdout, "KeyCode >> %x\n", e.xkey.keycode);
            // Excape Key Press -- valid for my keyboard layout
            if (e.xkey.keycode == 0x09) { break; }

            // React for space bar -- valid for my keyboard layout
            if (e.xkey.keycode == 0x41)
            {
                flag++;
                flag = flag % 3;
                draw = 1;
            }
        }

        // DEBUG
        printf("Draw: %d\n", draw);

        // Draw Routine
        if (draw == 1)
        {
            for (unsigned int h = 0; h < height; h++)
            {
                if (flag == 0)
                {
                    XAllocNamedColor(display, cmap, "Purple", &color, &trueColor);
                } else if (flag == 1)
                {
                    XAllocNamedColor(display, cmap, "Green", &color, &trueColor);
                } else
                {
                    XAllocNamedColor(display, cmap, "Gray", &color, &trueColor);
                }

                color.flags = DoRed | DoGreen | DoBlue;
                XAllocColor(display, cmap, &color);
                XSetForeground(display, gc, color.pixel);

                for (unsigned int w = 0; w < width; w++)
                {
                    XDrawPoint(display, win, gc, w, h);
                }
            }

            draw = 0;
        }

        //XFlush(display);
    }

    XCloseDisplay(display);

    return 0;
}