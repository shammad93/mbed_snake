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

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

//****************************************************************************************************
void TFT_4DGL :: circle(int x, int y , int radius, int color) {   // draw a circle in (x,y)
    char command[9]= "";

    command[0] = CIRCLE;

    command[1] = (x >> 8) & 0xFF;
    command[2] = x & 0xFF;

    command[3] = (y >> 8) & 0xFF;
    command[4] = y & 0xFF;

    command[5] = (radius >> 8) & 0xFF;
    command[6] = radius & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[7] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[8] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 9);
}

//****************************************************************************************************
void TFT_4DGL :: triangle(int x1, int y1 , int x2, int y2, int x3, int y3, int color) {   // draw a traingle
    char command[15]= "";

    command[0] = TRIANGLE;

    command[1] = (x1 >> 8) & 0xFF;
    command[2] = x1 & 0xFF;

    command[3] = (y1 >> 8) & 0xFF;
    command[4] = y1 & 0xFF;

    command[5] = (x2 >> 8) & 0xFF;
    command[6] = x2 & 0xFF;

    command[7] = (y2 >> 8) & 0xFF;
    command[8] = y2 & 0xFF;

    command[9] = (x3 >> 8) & 0xFF;
    command[10] = x3 & 0xFF;

    command[11] = (y3 >> 8) & 0xFF;
    command[12] = y3 & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;               // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;               // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;               // get blue on 5 bits

    command[13] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[14] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 15);
}

//****************************************************************************************************
void TFT_4DGL :: line(int x1, int y1 , int x2, int y2, int color) {   // draw a line
    char command[11]= "";

    command[0] = LINE;

    command[1] = (x1 >> 8) & 0xFF;
    command[2] = x1 & 0xFF;

    command[3] = (y1 >> 8) & 0xFF;
    command[4] = y1 & 0xFF;

    command[5] = (x2 >> 8) & 0xFF;
    command[6] = x2 & 0xFF;

    command[7] = (y2 >> 8) & 0xFF;
    command[8] = y2 & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;               // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;               // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;               // get blue on 5 bits

    command[9] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;   // first part of 16 bits color
    command[10] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 11);
}

//****************************************************************************************************
void TFT_4DGL :: rectangle(int x1, int y1 , int x2, int y2, int color) {   // draw a rectangle
    char command[11]= "";

    command[0] = RECTANGLE;

    command[1] = (x1 >> 8) & 0xFF;
    command[2] = x1 & 0xFF;

    command[3] = (y1 >> 8) & 0xFF;
    command[4] = y1 & 0xFF;

    command[5] = (x2 >> 8) & 0xFF;
    command[6] = x2 & 0xFF;

    command[7] = (y2 >> 8) & 0xFF;
    command[8] = y2 & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;               // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;               // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;               // get blue on 5 bits

    command[9] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;   // first part of 16 bits color
    command[10] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 11);
}

//****************************************************************************************************
void TFT_4DGL :: ellipse(int x, int y , int radius_x, int radius_y, int color) {   // draw an ellipse
    char command[11]= "";

    command[0] = ELLIPSE;

    command[1] = (x >> 8) & 0xFF;
    command[2] = x & 0xFF;

    command[3] = (y >> 8) & 0xFF;
    command[4] = y & 0xFF;

    command[5] = (radius_x >> 8) & 0xFF;
    command[6] = radius_x & 0xFF;

    command[7] = (radius_y >> 8) & 0xFF;
    command[8] = radius_y & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;               // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;               // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;               // get blue on 5 bits

    command[9] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;   // first part of 16 bits color
    command[10] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 11);
}

//****************************************************************************************************
void TFT_4DGL :: pixel(int x, int y, int color) {   // draw a pixel
    char command[7]= "";

    command[0] = PIXEL;

    command[1] = (x >> 8) & 0xFF;
    command[2] = x & 0xFF;

    command[3] = (y >> 8) & 0xFF;
    command[4] = y & 0xFF;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[5] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[6] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 7);
}

//******************************************************************************************************
int TFT_4DGL :: read_pixel(int x, int y) { // read screen info and populate data

    char command[5]= "";

    command[0] = READPIXEL;

    command[1] = (x >> 8) & 0xFF;
    command[2] = x & 0xFF;

    command[3] = (y >> 8) & 0xFF;
    command[4] = y & 0xFF;

    int i, temp = 0, color = 0, resp = 0;
    char response[2] = "";

    freeBUFFER();

    for (i = 0; i < 5; i++) {                   // send all chars to serial port
        writeBYTE(command[i]);
    }

    while (!_cmd.readable()) wait_ms(TEMPO);    // wait for screen answer

    while (_cmd.readable() && resp < ARRAY_SIZE(response)) {
        temp = _cmd.getc();
        response[resp++] = (char)temp;
    }

    color = ((response[0] << 8) + response[1]);

    return color; // WARNING : this is 16bits color, not 24bits... need to be fixed
}

//******************************************************************************************************
void TFT_4DGL :: screen_copy(int xs, int ys , int xd, int yd , int width, int height) {

    char command[13]= "";

    command[0] = SCREENCOPY;

    command[1] = (xs >> 8) & 0xFF;
    command[2] = xs & 0xFF;

    command[3] = (ys >> 8) & 0xFF;
    command[4] = ys & 0xFF;

    command[5] = (xd >> 8) & 0xFF;
    command[6] = xd & 0xFF;

    command[7] = (yd >> 8) & 0xFF;
    command[8] = yd & 0xFF;

    command[9] = (width >> 8) & 0xFF;
    command[10] = width & 0xFF;

    command[11] = (height >> 8) & 0xFF;
    command[12] = height & 0xFF;

    writeCOMMAND(command, 13);
}

//****************************************************************************************************
void TFT_4DGL :: pen_size(char mode) {   // set pen to SOLID or WIREFRAME
    char command[2]= "";

    command[0] = PENSIZE;
    command[1] = mode;

    writeCOMMAND(command, 2);
}