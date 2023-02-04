//
// TFT_4DGL is a class to drive 4D Systems TFT touch screens
//
// Copyright (C) <2010> Stephane ROCHON <stephane.rochon at free.fr>
//
// TFT_4DGL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TFT_4DGL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TFT_4DGL.  If not, see <http://www.gnu.org/licenses/>.

// @author Stephane Rochon

#include "mbed.h"

// Debug Verbose on terminal enabled
#ifndef DEBUGMODE
#define DEBUGMODE 1
#endif

// Common WAIT value in millisecond
#define TEMPO 5

// 4DGL Functions values
#define AUTOBAUD     '\x55'
#define CLS          '\x45'
#define BAUDRATE     '\x51'
#define VERSION      '\x56'
#define BCKGDCOLOR   '\x42'
#define DISPCONTROL  '\x59'
#define SETVOLUME    '\x76'
#define CIRCLE       '\x43'
#define TRIANGLE     '\x47'
#define LINE         '\x4C'
#define RECTANGLE    '\x72'
#define ELLIPSE      '\x65'
#define PIXEL        '\x50'
#define READPIXEL    '\x52'
#define SCREENCOPY   '\x63'
#define PENSIZE      '\x70'
#define SETFONT      '\x46'
#define TEXTMODE     '\x4F'
#define TEXTCHAR     '\x54'
#define GRAPHCHAR    '\x74'
#define TEXTSTRING   '\x73'
#define GRAPHSTRING  '\x53'
#define TEXTBUTTON   '\x62'
#define GETTOUCH     '\x6F'
#define WAITTOUCH    '\x77'
#define SETTOUCH     '\x75'


// Screen answers
#define ACK          '\x06'
#define NAK          '\x15'

// Screen states
#define OFF          '\x00'
#define ON           '\x01'

// Graphics modes
#define SOLID        '\x00'
#define WIREFRAME    '\x01'

// Text modes
#define TRANSPARENT  '\x00'
#define OPAQUE       '\x01'

// Fonts Sizes
#define FONT_5X7     '\x00'
#define FONT_8X8     '\x01'
#define FONT_8X12    '\x02'
#define FONT_12X16   '\x03'

// Touch Values
#define WAIT         '\x00'
#define PRESS        '\x01'
#define RELEASE      '\x02'
#define MOVE         '\x03'
#define STATUS       '\x04'
#define GETPOSITION  '\x05'

// Data speed
#define BAUD_110     '\x00'
#define BAUD_300     '\x01'
#define BAUD_600     '\x02'
#define BAUD_1200    '\x03'
#define BAUD_2400    '\x04'
#define BAUD_4800    '\x05'
#define BAUD_9600    '\x06'
#define BAUD_14400   '\x07'
#define BAUD_19200   '\x09'
#define BAUD_31250   '\x09'
#define BAUD_38400   '\x0A'
#define BAUD_56000   '\x0B'
#define BAUD_57600   '\x0C'
#define BAUD_115200  '\x0D'
#define BAUD_128000  '\x0E'
#define BAUD_256000  '\x0F'

// Defined Colors
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF
#define LGREY 0xBFBFBF
#define DGREY 0x5F5F5F

// Mode data
#define BACKLIGHT    '\x00'
#define DISPLAY      '\x01'
#define CONTRAST     '\x02'
#define POWER        '\x03'
#define ORIENTATION  '\x04'
#define TOUCH_CTRL   '\x05'
#define IMAGE_FORMAT '\x06'
#define PROTECT_FAT  '\x08'

// change this to your specific screen (newer versions) if needed
// Startup orientation is PORTRAIT so SIZE_X must be lesser than SIZE_Y
#define SIZE_X       240
#define SIZE_Y       320

#define IS_LANDSCAPE 0
#define IS_PORTRAIT  1

// Screen orientation
#define LANDSCAPE    '\x01'
#define LANDSCAPE_R  '\x02'
#define PORTRAIT     '\x03'
#define PORTRAIT_R   '\x04'

// Parameters
#define ENABLE       '\x00'
#define DISABLE      '\x01'
#define RESET        '\x02'

#define NEW          '\x00'
#define OLD          '\x01'

#define DOWN         '\x00'
#define UP           '\x01'

#define PROTECT      '\x00'
#define UNPROTECT    '\x02'

//**************************************************************************
// \class TFT_4DGL TFT_4DGL.h
// \brief This is the main class. It shoud be used like this : TFT_4GDL myLCD(p9,p10,p11);
/**
Example:
* @code
* // Display a white circle on the screen
* #include "mbed.h"
* #include " TFT_4DGL.h"
* 
* TFT_4GDL myLCD(p9,p10,p11);
* 
* int main() {
*     myLCD.circle(120, 160, 80, WHITE);
* }
* @endcode
*/

class TFT_4DGL {

public :

    TFT_4DGL(PinName tx, PinName rx, PinName rst);

// General Commands *******************************************************************************

/** Clear the entire screen using the current background colour */
    void cls();

/** Reset screen */
    void reset();
    
/** Launch Autobaud for serial communication. This function is automatically called at startup */
    void autobaud();
/** Set serial Baud rate (both sides : screen and mbed)
* @param Speed Correct BAUD value (see TFT_4DGL.h)
*/   
    void baudrate(int speed);

/** Set background colour to the specified value
* @param color in HEX RGB like 0xFF00FF
*/
    void background_color(int color);

/** Set screen display mode to specific values
* @param mode See 4DGL documentation
* @param value See 4DGL documentation
*/
    void display_control(char mode, char value);

/** Set internal speaker to specified value
* @param value Correct range is 8 - 127
*/
    void set_volume(char value);

// Graphics Commands *******************************************************************************

/** Draw a circle centered at x,y with a radius and a colour. It uses Pen Size stored value to draw a solid or wireframe circle
* @param x Horizontal position of the circle centre
* @param y Vertical position of the circle centre
* @param radius Radius of the circle
* @param color Circle color in HEX RGB like 0xFF00FF
*/
    void circle(int x , int y , int radius, int color);

    void triangle(int, int, int, int, int, int, int);
    void line(int, int, int, int, int);
    void rectangle(int, int, int, int, int);
    void ellipse(int, int, int, int, int);
    void pixel(int, int, int);
    int  read_pixel(int, int);
    void screen_copy(int, int, int, int, int, int);
    void pen_size(char);

// Texts Commands
    void set_font(char);
    void text_mode(char);
    void text_char(char, char, char, int);
    void graphic_char(char, int, int, int, char, char);
    void text_string(char *, char, char, char, int);
    void graphic_string(char *, int, int, char, int, char, char);
    void text_button(char *, char, int, int, int, char, int, char, char);

    void locate(char, char);
    void color(int);
    void putc(char);
    void puts(char *);

// Touch Command
    void touch_mode(char);
    void get_touch(int *, int *);
    void wait_touch(int);
    void set_touch(int, int, int, int);
    int  touch_status(void);

// Screen Data
    int type;
    int revision;
    int firmware;
    int reserved1;
    int reserved2;

// Text data
    char current_col;
    char current_row;
    int  current_color;
    char current_font;
    char current_orientation;
    char max_col;
    char max_row;

protected :

    Serial     _cmd;
    DigitalOut _rst;

    void freeBUFFER  (void);
    void writeBYTE   (char);
    int  writeCOMMAND(char *, int);
    int  readVERSION (char *, int);
    void getTOUCH    (char *, int, int *,int *);
    int  getSTATUS   (char *, int);
    void version     (void);
#if DEBUGMODE
    Serial pc;
#endif // DEBUGMODE
};

typedef unsigned char BYTE;