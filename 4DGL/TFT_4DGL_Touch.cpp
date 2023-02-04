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

//******************************************************************************************************
void TFT_4DGL :: touch_mode(char mode) { // Send touch mode (WAIT, PRESS, RELEASE or MOVE)

    char command[2]= "";

    command[0] = GETTOUCH;
    command[1] = mode;

    writeCOMMAND(command, 2);
}

//******************************************************************************************************
void TFT_4DGL :: get_touch(int *x, int *y) { // Get the touch coordinates

    char command[2] = "";
    
    command[0] = GETTOUCH;
    command[1] = GETPOSITION;
    
    getTOUCH(command, 2, x, y);
}

//******************************************************************************************************
int TFT_4DGL :: touch_status(void) { // Get the touch screen status

    char command[2] = "";
    
    command[0] = GETTOUCH;
    command[1] = STATUS;
    
    return getSTATUS(command, 2);
}


//******************************************************************************************************
void TFT_4DGL :: wait_touch(int delay) { // wait until touch within a delay in milliseconds

    char command[3]= "";

    command[0] = WAITTOUCH;

    command[1] = (delay >> 8) & 0xFF;
    command[2] = delay & 0xFF;

    writeCOMMAND(command, 3);
}

//******************************************************************************************************
void TFT_4DGL :: set_touch(int x1, int y1 , int x2, int y2) { // define touch area

    char command[9]= "";

    command[0] = SETTOUCH;

    command[1] = (x1 >> 8) & 0xFF;
    command[2] = x1 & 0xFF;

    command[3] = (y1 >> 8) & 0xFF;
    command[4] = y1 & 0xFF;

    command[5] = (x2 >> 8) & 0xFF;
    command[6] = x2 & 0xFF;

    command[7] = (y2 >> 8) & 0xFF;
    command[8] = y2 & 0xFF;

    writeCOMMAND(command, 9);
}