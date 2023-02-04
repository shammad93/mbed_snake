#ifndef SNAKE_H
#define SNAKE_H

//snake class
class snake
{
public:
    int x;
    int y;
    int size;
    int color;

    void pos(int tx, int ty);
    void draw(void);
    void undraw(void);
    snake(int tx, int ty, int tsize, int tcolor);
};

#endif