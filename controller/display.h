#ifndef DISPLAY_H
#define DISPLAY_H

// Display size in columns and rows
#define DISP_COLUMNS           16
#define DISP_ROWS              2

// Initializes the RGB display 
// returns: truth value of success
bool setup_display(void);

// Sets the backlight RGB
// r, g, b: 0-255 for each color channel, where 255 is maximum
void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b);

// Interpolates the displays current RGB to a new RGB value.
// r, g, b: 0-255 for each channel, 255 is maximum 
// step: amount to step each channel by
// must be called multiple times (i.e during tick) to work
void smooth_display_rgb(unsigned char r,
                        unsigned char g,
                        unsigned char b,
                        unsigned int step);


// Writes text to the display as a position 
// text: string to print 
// x, y: position in columns and rows, starting at the top left
void display_write(const char *text, int x, int y);

// Clears the screen of all text
void display_clear(void);

#endif//DISPLAY_H
