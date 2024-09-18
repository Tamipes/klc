#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv)
{
    char lang_codes[2][3] ={"hu","en"}; 
    XEvent e;
    Display *d;

    if (!(d = XOpenDisplay(NULL))) {
            fprintf(stderr, "cannot open display\n");
            return 1;
    }

    XKeysymToKeycode(d, XK_F1);

    int xkbEventType;
    XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
    XkbSelectEvents(d, XkbUseCoreKbd, XkbAllEventsMask, XkbAllEventsMask);

    XSync(d, False);

    int previ = -1;
    while (1) {
        XNextEvent(d, &e);
        if (e.type == xkbEventType) {
            XkbEvent* xkbEvent = (XkbEvent*) &e;
            if (xkbEvent->any.xkb_type == XkbStateNotify) {
                int lang = xkbEvent->state.group;
                if(previ == lang)
                    continue;
                previ = lang;
                if (lang == 1) {
                    fprintf(stdout, "%s\n", lang_codes[1]);
                    fflush(stdout);
                } else {
                    // fprintf(stdout, "0\n");
                    fprintf(stdout, "%s\n", lang_codes[0]);
                    fflush(stdout);
                }
            }
        }
    }

    return(0);
}
