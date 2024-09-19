//Source 1: https://stackoverflow.com/a/48407438
//Source 2: https://gist.github.com/fikovnik/ef428e82a26774280c4fdf8f96ce8eeb

// The rewritten part is under this license:
// Copyright (c) 2024 Tamipes

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentati
// on files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
// the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Sof
// tware.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WAR
// RANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYR
// IGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, A
// RISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

void print_lang();

XEvent e;
Display *d;

XkbRF_VarDefsRec vd;
XkbStateRec state;

int main(int argc, char **argv)
{
    if (!(d = XOpenDisplay(NULL))) {
        printf("cannot open display\n");
        return 1;
    }

    XKeysymToKeycode(d, XK_F1);

    int xkbEventType;
    XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
    XkbSelectEvents(d, XkbUseCoreKbd, XkbAllEventsMask, XkbAllEventsMask);

    XSync(d, False);

    print_lang();

    int previ = -1;
    while (1) {
        XNextEvent(d, &e);
        if (e.type == xkbEventType) {
            XkbEvent* xkbEvent = (XkbEvent*) &e;
            if (xkbEvent->any.xkb_type == XkbStateNotify) {
                // Handling the layout change event
                int lang = xkbEvent->state.group;
                if(previ == lang)
                    continue;
                previ = lang;
                print_lang();
            }
        }
    }

    XCloseDisplay(d);
    return(0);
}

void print_lang(){
    XkbGetState(d, XkbUseCoreKbd, &state);
    XkbRF_GetNamesProp(d, NULL, &vd);

    char *tok = strtok(vd.layout, ",");

    for (int i = 0; i < state.group; i++) {
        tok = strtok(NULL, ",");
        if (tok == NULL) {
            printf("ERROR:\tTami; should not happen???\n");
            return 1;
        }
    }
    fprintf(stdout,"%s\n", tok);
}
/*
//Source: https://gist.github.com/fikovnik/ef428e82a26774280c4fdf8f96ce8eeb
// compile with `gcc -I/usr/include getxkblayout.c -lX11 -lxkbfile`

int asda(int argc, char **argv) {
  Display *dpy = XOpenDisplay(NULL);

  if (dpy == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  XkbStateRec state;
  XkbGetState(dpy, XkbUseCoreKbd, &state);

  XkbDescPtr desc = XkbGetKeyboard(dpy, XkbAllComponentsMask, XkbUseCoreKbd);
  char *group = XGetAtomName(dpy, desc->names->groups[state.group]);
  printf("Full name: %s\n", group);

  XkbRF_VarDefsRec vd;
  XkbRF_GetNamesProp(dpy, NULL, &vd);

  char *tok = strtok(vd.layout, ",");

  for (int i = 0; i < state.group; i++) {
    tok = strtok(NULL, ",");
    if (tok == NULL) {
      return 1;
    }
  }

  printf("Layout name: %s\n", tok);
  XFree(group);
  XCloseDisplay(dpy);


  return 0;
}
*/
