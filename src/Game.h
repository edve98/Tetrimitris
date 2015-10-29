#ifndef GAME_H
#define GAME_H

#include "Block.h"

class Game{
  public:
    Game();
    void build();
    void update();
    void start();
    void input();
    void lockBlock();
    void pause();
    bool moveBlockDown();
    void moveBlockLeft();
    void moveBlockRight();
    bool stopGame();
    void checkLineClear();
    void lineClear(int);
    void lineClear();
    void loop();
    
    bool blockMoved;
    int keystroke;
    int score = 0;
    int gameSpeed = 750;
    int level = 1;
    bool GridIsOccupied[22][10];
    char nextBlock;
    char hold = 'o';
    bool holdUsed = false;
    bool holdUsedNow = false;
};

#endif