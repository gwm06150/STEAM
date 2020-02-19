#ifndef DISPLAY_H
#define DISPLAY_H

// Initializes the RGB display 
bool setup_display(void);

// Sets the backlight RGB
// 255 is maximum brightness, 0 is no brightness
void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b);

// Writes 'text' to the display, at position (x, y)
// Coordinates start in the top left corner of the display
void display_write(const char *text, int x, int y);

void display_clear(void);

#endif//DISPLAY_H
