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

#include "mbed.h"
#include "TFT_4DGL.h"

//****************************************************************************************************
void TFT_4DGL :: set_font(char mode) {   // set font size
    char command[2]= "";

    int w, h, fx = 8, fy = 8;

    command[0] = SETFONT;
    command[1] = mode;

    current_font = mode;

    if (current_orientation == IS_PORTRAIT) {
        w = SIZE_X;
        h = SIZE_Y;
    } else {
        w = SIZE_Y;
        h = SIZE_X;
    }

    switch (mode) {
        case FONT_5X7 :
            fx = 6;
            fy = 8;
            break;
        case FONT_8X8 :
            fx = 8;
            fy = 8;
            break;
        case FONT_8X12 :
            fx = 8;
            fy = 12;
            break;
        case FONT_12X16 :
            fx = 12;
            fy = 16;
            break;
    }

    max_col = w / fx;
    max_row = h / fy;

    writeCOMMAND(command, 2);
}

//****************************************************************************************************
void TFT_4DGL :: text_mode(char mode) {   // set text mode
    char command[2]= "";

    command[0] = TEXTMODE;
    command[1] = mode;

    writeCOMMAND(command, 2);
}

//****************************************************************************************************
void TFT_4DGL :: text_char(char c, char col, char row, int color) {   // draw a text char
    char command[6]= "";

    command[0] = TEXTCHAR;

    command[1] = c;
    command[2] = col;
    command[3] = row;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[4] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[5] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 8);
}

//****************************************************************************************************
void TFT_4DGL :: graphic_char(char c, int x, int y, int color, char width, char height) {   // draw a graphic char
    char command[10]= "";

    command[0] = GRAPHCHAR;

    command[1] = c;

    command[2] = (x >> 8) & 0xFF;
    command[3] = x & 0xFF;

    command[4] = (y >> 8) & 0xFF;
    command[5] = y & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[6] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[7] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    command[8] = width;

    command[9] = height;

    writeCOMMAND(command, 10);
}

//****************************************************************************************************
void TFT_4DGL :: text_string(char *s, char col, char row, char font, int color) {   // draw a text string

    char command[1000]= "";
    int size = strlen(s);
    int i = 0;

    command[0] = TEXTSTRING;

    command[1] = col;
    command[2] = row;

    command[3] = font;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[4] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[5] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    for (i=0; i<size; i++) command[6+i] = s[i];

    command[6+size] = 0;

    writeCOMMAND(command, 7 + size);
}

//****************************************************************************************************
void TFT_4DGL :: graphic_string(char *s, int x, int y, char font, int color, char width, char height) {   // draw a text string

    char command[1000]= "";
    int size = strlen(s);
    int i = 0;

    command[0] = GRAPHSTRING;

    command[1] = (x >> 8) & 0xFF;
    command[2] = x & 0xFF;

    command[3] = (y >> 8) & 0xFF;
    command[4] = y & 0xFF;

    command[5] = font;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[6] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[7] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    command[8] = width;

    command[9] = height;

    for (i=0; i<size; i++) command[10+i] = s[i];

    command[10+size] = 0;

    writeCOMMAND(command, 11 + size);
}

//****************************************************************************************************
void TFT_4DGL :: text_button(char *s, char mode, int x, int y, int button_color, char font, int text_color, char width, char height) {   // draw a text string

    char command[1000]= "";
    int size = strlen(s);
    int i = 0, red5, green6, blue5;

    command[0] = TEXTBUTTON;

    command[1] = mode;

    command[2] = (x >> 8) & 0xFF;
    command[3] = x & 0xFF;

    command[4] = (y >> 8) & 0xFF;
    command[5] = y & 0xFF;

    red5   = (button_color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    green6 = (button_color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    blue5  = (button_color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[6] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[7] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    command[8] = font;

    red5   = (text_color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    green6 = (text_color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    blue5  = (text_color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[9] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[10] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    command[11] = width;

    command[12] = height;

    for (i=0; i<size; i++) command[13+i] = s[i];

    command[13+size] = 0;

    writeCOMMAND(command, 14 + size);
}

//****************************************************************************************************
void TFT_4DGL :: locate(char col, char row) {   // place text curssor at col, row
    current_col = col;
    current_row = row;
}

//****************************************************************************************************
void TFT_4DGL :: color(int color) {   // set text color
    current_color = color;
}

//****************************************************************************************************
void TFT_4DGL :: putc(char c) {   // place char at current cursor position

    text_char(c, current_col++, current_row, current_color);

    if (current_col == max_col) {
        current_col = 0;
        current_row++;
    }
    if (current_row == max_row) {
        current_row = 0;
    }
}

//****************************************************************************************************
void TFT_4DGL :: puts(char *s) {   // place string at current cursor position

    text_string(s, current_col, current_row, current_font, current_color);

    current_col += strlen(s);

    if (current_col >= max_col) {
        current_row += current_col / max_col;
        current_col %= max_col;
    }
    if (current_row >= max_row) {
        current_row %= max_row;
    }
}