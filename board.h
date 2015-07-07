#ifndef _BOARD_H_
#define _HOARD_H_

#include <vector>
#include <deque>
#include <set>

const int DEF_SIZE = 100;
const int DEF_LENGTH = 7;
const int DEF_NUM_APPLES = 100;
const int DEF_TIMER = 100;

enum Direction {UP, DOWN, LEFT, RIGHT};
enum Status {EMPTY, SNAKE, APPLE};

struct Point {
    int x;
    int y;

    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int xx, int yy)
    {
        x = xx;
        y = yy;
    }
    bool operator==(Point p)
    {
        return x == p.x && y == p.y;
    }
    bool operator<(Point p)
    {
        if (y < p.y)
            return true;
        else
            return x < p.x;
    }
    Point& operator=(const Point& p)
    {
        x = p.x;
        y = p.y;
    }
    bool operator()(Point p1, Point p2)
    {
        return p1 < p2;
    }
};

class Board {
public:
    Board();
    Board(int);
    Board(int, int);
    void setDirection(Direction);
    void move();
    void togglePause();
    void restart();
    const bool isPaused() const;
    const bool isStopped() const;
    void draw();
    void draw3D();
    const int getWidth() const;
    const int getHeight() const;
    const int getPeriod() const;
private:
    bool pause;
    bool stop;
    int width;
    int height;
    int period;
    Direction lastd;
    Direction direct;
    Point shead;
    Point stail;
    std::vector<std::vector<Status> > grids;
    std::set<Point, Point> empty;
    std::set<Point, Point> apple;
    std::deque<Point> snake;

    int slength;
    int apples;

    void setApple(int);
    void init();
    void initGrids();
    void initSnake();
};

#endif
