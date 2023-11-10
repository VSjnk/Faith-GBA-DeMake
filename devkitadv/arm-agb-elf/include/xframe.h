#ifndef _XFRAME_H
#define _XFRAME_H

void ex_start_tty  (char *screen, const unsigned char *fontdata, int width, int height);
void ex_redraw_tty (void);
int  ex_poll_tty   (void);

extern const unsigned char ex_font_fixedsys[];

#endif // _XFRAME_H



