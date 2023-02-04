#include "mbed.h"
#include "stdlib.h"
#include "TFT_4DGL.h"
#include "mpr121.h"
#include <list>
#include "snake.h"

using namespace std;

// overwrite 4DGL library screen size settings in TFT_4DGL.h
#define SIZE_X  479
#define SIZE_Y  639
//

InterruptIn interrupt(p26); // Create the interrupt receiver object on pin 26
TFT_4DGL vga(p9,p10,p11);   // serial tx, serial rx, reset pin;
I2C i2c(p28, p27);          // Setup the i2c bus on pins 28 and 27
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);  // Setup the Mpr121:



//Global Functions
int keyint(void);
//Global Constants
char const digit[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//Global frame count
int ticker;


int main()
{
    //display handshake
    vga.baudrate(115200);
    
    //added - Set Display to 640 by 480 mode
    vga.display_control(0x0c, 0x01);
    vga.background_color(BLACK);
    vga.set_font(FONT_8X8);
    vga.text_mode(TRANSPARENT);

    //Restart entry point
restart:

    vga.cls();


    //game variables
    list<snake> snakes;
    list<snake>::iterator it;
    bool quit = false;
    int points = 0;
    int x = 0;
    int y = 0;
    int foodx = (rand() % 73) + 2;
    int foody = (rand() % 52) + 4;
    int dir = 6;
    int last_dir = 0;

    //set up field
    vga.line(7,23,623, 23, WHITE);
    vga.line(7,23,7, 463, WHITE);
    vga.line(623,23, 623, 463, WHITE);
    vga.line(7, 463, 623, 463, WHITE);
    vga.text_string("SCORE:", 2, 1, FONT_8X8, WHITE);
    vga.text_char(digit[points%10], 9, 1, WHITE);

    //set frame ticker
    ticker = 0;
    //initialize timer class
    Timer timer;

    //seed random number generator
    srand(time(NULL));

    //start timer
    timer.start();

    //display first apple
    vga.rectangle((foodx*8)-1, (foody*8)-1 ,(foodx*8)+7, (foody*8)+7, RED);

    //create list of snake class
    for(int i =0; i < 30; i++)
        snakes.push_front(snake( 63 + (i*8), 127, 8, GREEN));

    //draw beginning snake parts
    for(it = snakes.begin(); it != snakes.end(); it++) {
        (*it).draw();
    }

    while( !quit ) {
        //start Time for fps limit
        timer.reset();

        //grab old head of snake's coordinates
        x = snakes.front().x;
        y = snakes.front().y;

        //create coordinates for new location of head
        
        //Guard to keep snake moving once per frame
        if(dir == 0)
            dir = last_dir;
        
        //movement control from input
        //Snake cannot turn directly backwards and is always moving
        if(dir == 1) {
            if (last_dir != 5) {
                y-=8;
                last_dir = dir;
            } else
                y+=8;
        } else if(dir == 6) {
            if(last_dir != 4) {
                x+=8;
                last_dir = dir;
            } else
                x-=8;
        } else if(dir == 5) {
            if(last_dir != 1) {
                y+=8;
                last_dir = dir;
            } else
                y-=8;
        } else if(dir == 4) {
            if(last_dir != 6) {
                x-=8;
                last_dir = dir;
            } else
                x+=8;
        }



        //add new head to the front
        snakes.push_front(snake(x, y, 8, GREEN));
        snake head = snakes.front();

        if( x == (foodx*8)-1 && y == (foody*8)-1) {//if new head location is an apple
            foodx = (rand() % 73) + 2;  //generate a new one and add points
            foody = (rand() % 52) + 4;
            vga.rectangle((foodx*8)-1, (foody*8)-1 ,(foodx*8)+7, (foody*8)+7, RED);

            //adjust score
            points++;
            vga.rectangle(8*8-1, 7, 10*8-1, 15, BLACK);
            vga.text_char(digit[points%10], 9, 1, WHITE);
            if(points > 9) {
                vga.text_char(digit[(points/10)%10], 8, 1, WHITE);
            }
            head.draw();    //draw new head leaving tail in place
        } else {
            snakes.back().undraw();         //remove tail piece because snake doesnt grow this frame
            head.draw();
            snakes.pop_back();              //if we havent aten an apple this frame we must remove the tail of the snake
        }

        //Collision with border "death"
        if( x <= 8 || x >= 615 || y <= 24 || y >= 455)
            quit = true;

        //Check for collision with self
        for(it = snakes.begin(); it != snakes.end(); it++) {
            if( (*it).y == y && (*it).x == x && it != snakes.begin() )
                quit = true; 
        }

        //force roughly 15 fps by waiting to process next frame
        do {        } while( timer.read_ms() < 16.5);

        //increment frame counter
        ticker++;
        //grab direction or keep last direction
        dir = keyint();

    }
    //ENDGAME
    //clear background for text displays
    vga.rectangle(183,223,455,255, BLACK);

    vga.graphic_string("You Lost The Game", 183, 223, FONT_8X8, WHITE, 2,2);
    vga.graphic_string("SCORE",247,239, FONT_8X8, WHITE, 2, 2);

    //print final points
    vga.graphic_char(digit[points%10], 362, 239, WHITE, 2, 2);
    if(points > 9) {
        vga.graphic_char(digit[(points/10)%10], 343 , 239, WHITE, 2, 2);
    }


    //flash prompt to user for restart
    dir = 0;
    while( dir != 5) {
        wait(.5);
        vga.rectangle(248,400,392,408, BLACK);
        wait(.5);
        dir = keyint();
        vga.graphic_string("Hold 5 To Restart", 248, 400, FONT_8X8, WHITE, 1, 1);
    }
    //if restart, force erase of snake list elements
    snakes.erase(snakes.begin(), snakes.end());
    goto restart;

    return 0;

}

//Main Library Functions
void snake::draw(void)  //Draw the snake part
{
    vga.rectangle(x, y, x+size, y+size, color);
}

void snake::undraw(void)//Overdraw snake part with background
{
    vga.rectangle(x, y, x+size, y+size, BLACK);
}


int keyint() //check for input on MPR121
{
    int value=mpr121.read(0x00); // LED demo mod by J. Hamblen

    //switch to return player input
    switch (value) {
        case 0x40://move right button 6
            return 6;

        case 0x10://move left button 4
            return 4;

        case 0x20://move down button 5
            return 5;

        case 0x02://move up button 1
            return 1;
    }
    //default case
    return 0;
}
