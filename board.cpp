#include <cstdlib>
#include <ctime>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "board.h"

Board::Board()
{
    width = DEF_SIZE;
    height = DEF_SIZE;
    init();
}

Board::Board(int n)
{
    width = n;
    height = n;
    init();
}

Board::Board(int m, int n)
{
    width = m;
    height = n;
    init();
}

void Board::setDirection(Direction d)
{
    if (lastd == LEFT && d == RIGHT ||
            lastd == RIGHT && d == LEFT ||
            lastd == UP && d == DOWN ||
            lastd == DOWN && d == UP)
        return;
    if (lastd == d && !pause && !stop)
        move();
    direct = d;
}

void Board::togglePause()
{
    pause = !pause;
}

const bool Board::isPaused() const
{
    return pause;
}

const bool Board::isStopped() const
{
    return stop;
}

void Board::restart()
{
    init();
}

void Board::move()
{
    Point next;
    lastd = direct;
    switch (direct) {
    case UP:
        next = Point(shead.x, shead.y + 1);
        break;
    case DOWN:
        next = Point(shead.x, shead.y - 1);
        break;
    case LEFT:
        next = Point(shead.x - 1, shead.y);
        break;
    case RIGHT:
        next = Point(shead.x + 1, shead.y);
        break;
    default:
        break;
    }

    if (next.x < 0 || next.x >= width ||
            next.y < 0 || next.y >= height ||
            grids[next.y][next.x] == SNAKE) {
        stop = true;
    } else if (grids[next.y][next.x] == APPLE) {
        slength++;
        apples++;
        if (period > 10)
            period--;
        shead = next;
        snake.push_front(shead);
        grids[shead.y][shead.x] = SNAKE;
        apple.erase(shead);
        setApple(1);
    } else {
        shead = next;
        grids[shead.y][shead.x] = SNAKE;
        grids[stail.y][stail.x] = EMPTY;
        snake.push_front(shead);
        empty.erase(shead);
        empty.insert(stail);
        snake.pop_back();
        stail = snake.back();
    }
}

void Board::init()
{
    initGrids();
    initSnake();
    setApple(DEF_NUM_APPLES);
    pause = true;
    stop = false;
    period = DEF_TIMER;
    apples = 0;
}

void Board::initGrids()
{
    grids.clear();
    for (int j = 0; j < height; j++) {
        vector<Status> v;
        for (int i = 0; i < width; i++)
            v.push_back(EMPTY);
        grids.push_back(v);
    }
}

void Board::initSnake()
{
    slength = DEF_LENGTH;
    int size = width*height;
    empty.clear();
    snake.clear();
    shead.x = width/2;
    shead.y = height/2;
    stail.x = shead.x - slength + 1;
    stail.y = shead.y;
    for (int i = 0; i < slength; i++) {
        grids[shead.y][shead.x - i] = SNAKE;
        snake.push_back(Point(shead.x - i, shead.y));
    }
    direct = RIGHT;
    lastd = RIGHT;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (grids[j][i] == EMPTY)
                empty.insert(Point(i, j));
        }
    }
}

void Board::setApple(int n)
{
    int size = empty.size();
    srand(time(0));
    for (int i = 0; i < n; i++) {
        if (size < 1)
            return;
        int offset = rand() % size;
        set<Point, Point>::iterator it = empty.begin();
        for (int i = 0; i < offset; i++)
            it++;
        grids[it->y][it->x] = APPLE;
        apple.insert(*it);
        empty.erase(*it);
        size--;
    }
}

void Board::draw3D()
{
    Point look;
    GLfloat position[] = {100, 0, 0, 1};
    gluLookAt(0, 50, 50, 0, 0, 0, 0, 1, 0);
    gluLookAt(shead.x + 0.5, shead.y + 0.5, 1,
            shead.x + 0.5, shead.y + 1.5, 1,
            0, 0, 1);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glColor3f(0.2, 0.5, 0.2);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, height, 0);
    glVertex3f(width, height, 0);
    glVertex3f(width, 0, 0);
    glEnd();
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            switch (grids[j][i]) {
            case SNAKE:
                glColor3f(0.2, 0.2, 1.0);
                glPushMatrix();
                glTranslatef(i + 0.5, j + 0.5, 0.5);
                glutSolidCube(1);
                glPopMatrix();
                break;
            case APPLE:
                glColor3f(1.0, 0.2, 0.2);
                glPushMatrix();
                glTranslatef(i + 0.5, j + 0.5, 0.5);
                glutSolidSphere(0.5, 8, 8);
                glPopMatrix();
                break;
            default:
                break;
            }
        }
    }
}

const int Board::getWidth() const
{
    return width;
}

const int Board::getHeight() const
{
    return height;
}

const int Board::getPeriod() const
{
    return period;
}
