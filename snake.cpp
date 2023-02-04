#include "snake.h"
#include "TFT_4DGL.h"

//snake constructor
snake::snake(int tx, int ty, int tsize, int tcolor)
{
    x = tx;
    y = ty;
    size = tsize;
    color = tcolor;
}

void snake::pos(int tx, int ty)
{
    x = tx;
    y = ty;
}

