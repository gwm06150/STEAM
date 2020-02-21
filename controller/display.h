#ifndef DISPLAY_H
#define DISPLAY_H

// Initializes the RGB display 
bool setup_display(void);

// Sets the backlight RGB
// 255 is maximum brightness, 0 is no brightness
void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b);
// Causes the backlight RGB to "approach" the 
// desired RGB based on a divisor. Smaller divisors
// result in faster transitions. Must be called repeatedly
void lerp_display_rgb(unsigned char r,
                      unsigned char g,
                      unsigned char b,
                      unsigned int divisor);

// Writes 'text' to the display, at position (x, y)
// Coordinates start in the top left corner of the display
void display_write(const char *text, int x, int y);
// Clears the screen of all text
void display_clear(void);

#endif//DISPLAY_H
