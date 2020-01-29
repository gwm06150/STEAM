#ifndef DISPLAY_H
#define DISPLAY_H

bool setup_display(void);

void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b);

void display_write(const char *text, int x, int y);

#endif//DISPLAY_H
