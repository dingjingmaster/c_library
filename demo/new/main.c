#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

struct conky_window {
    Window root, window, desktop;
    Drawable drawable;
    Visual *visual;
    Colormap colourmap;
    GC gc;
    long border_inner_margin, border_outer_margin, border_width;

#ifdef HAVE_XDBE
    XdbeBackBuffer back_buffer;
#endif
#ifdef XFT
    XftDraw *xftdraw;
#endif

    int width;
    int height;
#ifdef OWN_WINDOW
    char class_name[256];
	char title[256];
	int x;
	int y;
	unsigned int type;
	unsigned long hints;
#endif
};


int main(int argc, char* argv[])
{
    struct conky_window window;

    Display *display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    int display_width = DisplayWidth(display, screen);
    int display_height = DisplayHeight(display, screen);

    Window root = RootWindow(display, screen);
    window.colourmap = XCreateColormap(display, DefaultRootWindow(display), window.visual, AllocNone);

    XSetWindowAttributes attrs = { ParentRelative, 0L, 0, 0L, 0, 0,
                                   Always, 0L, 0L, False, StructureNotifyMask | ExposureMask, 0L,
                                   True, 0, 0 };


    attrs.colormap = window.colourmap;
    int flags = CWBorderPixel | CWColormap | CWOverrideRedirect;

    window.window = XCreateWindow(display, window.desktop, 100,
                                  100, 600, 600, 0, 32, InputOutput, 1, flags, &attrs);
    XLowerWindow(display, window.window);

    // 输出
    printf("width:%d, height:%d\n", display_width, display_height);
//    display
    return 0;
}
