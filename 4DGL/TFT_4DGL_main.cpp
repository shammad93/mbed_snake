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

//Serial pc(USBTX,USBRX);

//******************************************************************************************************
TFT_4DGL :: TFT_4DGL(PinName tx, PinName rx, PinName rst) : _cmd(tx, rx), 
                                                            _rst(rst) 
#if DEBUGMODE
                                                            ,pc(USBTX, USBRX)
#endif // DEBUGMODE
{ // Constructor

#if DEBUGMODE
    pc.baud(115200);

    pc.printf("\n\n\n");
    pc.printf("********************\n");
    pc.printf("TFT_4DGL CONSTRUCTOR\n");
    pc.printf("********************\n");
#endif

    _rst = 1;    // put RESET pin to high to start TFT screen

    reset();
    autobaud();  // send autobaud command
    version();   // get version information
    cls();       // clear screen

    current_col         = 0;            // initial cursor col
    current_row         = 0;            // initial cursor row
    current_color       = WHITE;        // initial text color
    current_orientation = IS_PORTRAIT;  // initial screen orientation

    set_font(FONT_5X7);                 // initial font
    text_mode(OPAQUE);                  // initial texr mode
}

//******************************************************************************************************
void TFT_4DGL :: writeBYTE(char c) { // send a BYTE command to screen

    _cmd.putc(c);
    wait_ms(1);

#if DEBUGMODE
    pc.printf("   Char sent : 0x%02X\n",c);
#endif

}

//******************************************************************************************************
void TFT_4DGL :: freeBUFFER(void) {       // Clear serial buffer before writing command

    while (_cmd.readable()) _cmd.getc();  // clear buffer garbage
}

//******************************************************************************************************
int TFT_4DGL :: writeCOMMAND(char *command, int number) { // send several BYTES making a command and return an answer

#if DEBUGMODE
    pc.printf("\n");
    pc.printf("New COMMAND : 0x%02X\n", command[0]);
#endif
    int i, resp = 0;
    freeBUFFER();

    for (i = 0; i < number; i++) writeBYTE(command[i]); // send command to serial port

    while (!_cmd.readable()) wait_ms(TEMPO);              // wait for screen answer
    if (_cmd.readable()) resp = _cmd.getc();           // read response if any
    switch (resp) {
        case ACK :                                     // if OK return   1
            resp =  1;
            break;
        case NAK :                                     // if NOK return -1
            resp = -1;
            break;
        default :
            resp =  0;                                 // else return   0
            break;
    }
#if DEBUGMODE
    pc.printf("   Answer received : %d\n",resp);
#endif

    return resp;
}

//**************************************************************************
void TFT_4DGL :: reset() {  // Reset Screen

    _rst = 0;               // put RESET pin to low
    wait_ms(TEMPO);         // wait a few milliseconds for command reception
    _rst = 1;               // put RESET back to high
    wait(3);                // wait 3s for screen to restart

    freeBUFFER();           // clean buffer from possible garbage
}

//**************************************************************************
void TFT_4DGL :: autobaud() { // send AutoBaud command (9600)
    char command[1] = "";
    command[0] = AUTOBAUD;
    writeCOMMAND(command, 1);
}

//**************************************************************************
void TFT_4DGL :: cls() {  // clear screen
    char command[1] = "";
    command[0] = CLS;
    writeCOMMAND(command, 1);
}

//**************************************************************************
void TFT_4DGL :: version() {  // get API version
    char command[2] = "";
    command[0] = VERSION;
    command[1] = OFF;
    readVERSION(command, 2);
}

//**************************************************************************
void TFT_4DGL :: baudrate(int speed) {  // set screen baud rate
    char command[2]= "";
    command[0] = BAUDRATE;
    switch (speed) {
        case  110 :
            command[1] = BAUD_110;
            break;
        case  300 :
            command[1] = BAUD_300;
            break;
        case  600 :
            command[1] = BAUD_600;
            break;
        case 1200 :
            command[1] = BAUD_1200;
            break;
        case 2400 :
            command[1] = BAUD_2400;
            break;
        case 4800 :
            command[1] = BAUD_4800;
            break;
        case 9600 :
            command[1] = BAUD_9600;
            break;
        case 14400 :
            command[1] = BAUD_14400;
            break;
        case 19200 :
            command[1] = BAUD_19200;
            break;
        case 31250 :
            command[1] = BAUD_31250;
            break;
        case 38400 :
            command[1] = BAUD_38400;
            break;
        case 56000 :
            command[1] = BAUD_56000;
            break;
        case 57600 :
            command[1] = BAUD_57600;
            break;
        case 115200 :
            command[1] = BAUD_115200;
            break;
        case 128000 :
            command[1] = BAUD_128000;
            break;
        case 256000 :
            command[1] = BAUD_256000;
            break;
        default   :
            command[1] = BAUD_9600;
            speed = 9600;
            break;
    }

    int i, resp = 0;

    freeBUFFER();

    for (i = 0; i <2; i++) writeBYTE(command[i]);      // send command to serial port
    _cmd.baud(speed);                                  // set mbed to same speed

    while (!_cmd.readable()) wait_ms(TEMPO);           // wait for screen answer

    if (_cmd.readable()) resp = _cmd.getc();           // read response if any
    switch (resp) {
        case ACK :                                     // if OK return   1
            resp =  1;
            break;
        case NAK :                                     // if NOK return -1
            resp = -1;
            break;
        default :
            resp =  0;                                 // else return   0
            break;
    }
}

//******************************************************************************************************
int TFT_4DGL :: readVERSION(char *command, int number) { // read screen info and populate data

    int i, temp = 0, resp = 0;
    char response[5] = "";

    freeBUFFER();

    for (i = 0; i < number; i++) writeBYTE(command[i]);    // send all chars to serial port

    while (!_cmd.readable()) wait_ms(TEMPO);               // wait for screen answer

    while (_cmd.readable() && resp < ARRAY_SIZE(response)) {
        temp = _cmd.getc();
        response[resp++] = (char)temp;
    }
    switch (resp) {
        case 5 :                                           // if OK populate data and return 1
            type      = response[0];
            revision  = response[1];
            firmware  = response[2];
            reserved1 = response[3];
            reserved2 = response[4];
            resp      = 1;
            break;
        default :
            resp =  0;                                     // else return 0
            break;
    }
    return resp;
}

//****************************************************************************************************
void TFT_4DGL :: background_color(int color) {            // set screen background color
    char command[3]= "";                                  // input color is in 24bits like 0xRRGGBB

    command[0] = BCKGDCOLOR;

    int red5   = (color >> (16 + 3)) & 0x1F;              // get red on 5 bits
    int green6 = (color >> (8 + 2))  & 0x3F;              // get green on 6 bits
    int blue5  = (color >> (0 + 3))  & 0x1F;              // get blue on 5 bits

    command[1] = ((red5 << 3)   + (green6 >> 3)) & 0xFF;  // first part of 16 bits color
    command[2] = ((green6 << 5) + (blue5 >>  0)) & 0xFF;  // second part of 16 bits color

    writeCOMMAND(command, 3);
}

//****************************************************************************************************
void TFT_4DGL :: display_control(char mode, char value) {   // set screen mode to value
    char command[3]= "";

    command[0] = DISPCONTROL;
    command[1] = mode;
    command[2] = value;

    if (mode ==  ORIENTATION) {
        switch (value) {
            case LANDSCAPE :
                current_orientation = IS_LANDSCAPE;
                break;
            case LANDSCAPE_R :
                current_orientation = IS_LANDSCAPE;
                break;
            case PORTRAIT :
                current_orientation = IS_PORTRAIT;
                break;
            case PORTRAIT_R :
                current_orientation = IS_PORTRAIT;
                break;
        }
    }
    writeCOMMAND(command, 3);
    set_font(current_font);
}

//****************************************************************************************************
void TFT_4DGL :: set_volume(char value) {   // set sound volume to value
    char command[2]= "";

    command[0] = SETVOLUME;
    command[1] = value;

    writeCOMMAND(command, 2);
}


//******************************************************************************************************
void TFT_4DGL :: getTOUCH(char *command, int number, int *x, int *y) { // read screen info and populate data

#if DEBUGMODE
    pc.printf("\n");
    pc.printf("New COMMAND : 0x%02X\n", command[0]);
#endif
    int i, temp = 0, resp = 0;
    char response[5] = "";

    freeBUFFER();

    for (i = 0; i < number; i++) writeBYTE(command[i]);    // send all chars to serial port

    while (!_cmd.readable()) wait_ms(TEMPO);               // wait for screen answer

    while (_cmd.readable() && resp < ARRAY_SIZE(response)) {
        temp = _cmd.getc();
        response[resp++] = (char)temp;
    }

#if DEBUGMODE
    pc.printf("   Answer received %d : 0x%02X 0x%02X 0x%02X 0x%02X\n", resp, response[0], response[1], response[2], response[3]);
#endif

    switch (resp) {
        case 4 :                                                              // if OK populate data
            *x = ((response[0]<<8)+ response[1]) * (response[0] != 0xFF);
            *y = ((response[2]<<8)+ response[3]) * (response[2] != 0xFF);
            break;
        default :
            *x = -1;
            *y = -1;
            break;
    }

#if DEBUGMODE
    pc.printf("   X,Y : %03d,%03d\n", *x, *y);
#endif
}

//******************************************************************************************************
int TFT_4DGL :: getSTATUS(char *command, int number) { // read screen info and populate data

#if DEBUGMODE
    pc.printf("\n");
    pc.printf("New COMMAND : 0x%02X\n", command[0]);
#endif

    int i, temp = 0, resp = 0;
    char response[5] = "";

    freeBUFFER();

    for (i = 0; i < number; i++) writeBYTE(command[i]);    // send all chars to serial port

    while (!_cmd.readable()) wait_ms(TEMPO);    // wait for screen answer

    while (_cmd.readable() && resp < ARRAY_SIZE(response)) {
        temp = _cmd.getc();
        response[resp++] = (char)temp;
    }
    switch (resp) {
        case 4 :
            resp = (int)response[1];         // if OK populate data
            break;
        default :
            resp =  -1;                      // else return   0
            break;
    }
    
#if DEBUGMODE
    pc.printf("   Answer received : %d\n", resp);
#endif

    return resp;
}