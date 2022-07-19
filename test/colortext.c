#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

struct {
    int width, height;
    char *text;
    int text_len;

    Display* display;
    int screen;

    Window root, window;
    GC gc;
    XFontStruct* font;
    unsigned long black_pixel, white_pixel;
    Colormap cmap;

    // Character Height and Width in Pixels
    int c_height;
    int c_width;
}
text_box;

static void x_connect()
{
    text_box.display = XOpenDisplay(NULL);
    if (!text_box.display)
    {
        fprintf(stderr, "Could not open Display\n");
        exit(1);
    }

    text_box.screen = DefaultScreen(text_box.display);
    text_box.root = RootWindow(text_box.display, text_box.screen);
    text_box.black_pixel = BlackPixel(text_box.display, text_box.screen);
    text_box.white_pixel = WhitePixel(text_box.display, text_box.screen);
}

static void create_window()
{
    text_box.width = 300;
    text_box.height = 300;

    int x = 0;
    int y = 0;
    int border = 0;

    text_box.window = XCreateSimpleWindow(text_box.display, text_box.root, x, y, text_box.width, text_box.height, border, text_box.black_pixel, text_box.white_pixel);
    XSelectInput(text_box.display, text_box.window, ExposureMask | KeyPressMask);
    XMapWindow(text_box.display, text_box.window);
}

static void set_up_gc()
{
    XGCValues values;
    values.foreground = text_box.white_pixel;
    values.background = text_box.black_pixel;

    text_box.gc = XCreateGC(text_box.display, text_box.window, (GCForeground | GCBackground), &values);
    XSetBackground(text_box.display, text_box.gc, text_box.white_pixel);
    XSetForeground(text_box.display, text_box.gc, text_box.black_pixel);
}

static void set_up_font()
{
    const char* fontname = "-misc-fixed-bold-r-normal--13-100-100-100-c-70-iso8859-1";
    text_box.font = XLoadQueryFont(text_box.display, fontname);
    // Backup if above fails
    if (!text_box.font)
    {
        fprintf(stderr, "Setting the backup font\n");
        text_box.font = XLoadQueryFont(text_box.display, "fixed");
    }
    XSetFont(text_box.display, text_box.gc, text_box.font->fid);
}

static void draw_screen(int flag)
{
    int x, y, direction, ascent, descent;
    XCharStruct overall;

    XColor color, trueColor;

    XTextExtents(text_box.font, text_box.text, text_box.text_len, &direction, &ascent, &descent, &overall);
    x = (text_box.width - overall.width) / 2;
    y = (text_box.height/2) + (ascent - descent)/2;
    XClearWindow(text_box.display, text_box.window);

    // Drawing Background
    XAllocNamedColor(text_box.display, text_box.cmap, "Green", &color, &trueColor);
    color.flags = DoRed | DoGreen | DoBlue;
    XSetForeground(text_box.display, text_box.gc, color.pixel);
    for (int h = y - (overall.ascent - overall.descent); h < y; h++)
    {
        for (int w = x; w < x+overall.width; w++)
        {
            XDrawPoint(text_box.display, text_box.window, text_box.gc, w, h);
        }
    }

    // Drawing String
    if (flag == 0)
    {
        XAllocNamedColor(text_box.display, text_box.cmap, "Purple", &color, &trueColor);
    } else if (flag == 1)
    {
        XAllocNamedColor(text_box.display, text_box.cmap, "Green", &color, &trueColor);
    } else
    {
        XAllocNamedColor(text_box.display, text_box.cmap, "Gray", &color, &trueColor);
    }
    color.flags = DoRed | DoGreen | DoBlue;
    XSetForeground(text_box.display, text_box.gc, color.pixel);
    XDrawString(text_box.display, text_box.window, text_box.gc, x, y, text_box.text, text_box.text_len);
}

static void event_loop()
{
    int flag = 0;
    while (1)
    {
        XEvent e;
        XNextEvent(text_box.display, &e);
        if (e.type == KeyPress)
        {
            // Escape Key to exit -- valid for my keyboard layout
            if (e.xkey.keycode == 0x09) { break; }

            // Space bar to change color -- valid for my keyboard layout
            if (e.xkey.keycode == 0x41)
            {
                flag++;
                flag = flag % 3;
            }
        }
        draw_screen(flag);
    }
}

int main()
{
    text_box.text = "Hello World!";
    text_box.text_len = strlen(text_box.text);

    x_connect();
    create_window();
    text_box.cmap = DefaultColormap(text_box.display, text_box.screen);
    set_up_gc();
    set_up_font();
    event_loop();

    return 0;
}