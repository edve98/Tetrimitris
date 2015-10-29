#include "Game.h"
#include "Block.h"

#include <ncurses.h>
#include <ctime>
#include <stdlib.h>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include <iostream>

Block CurrentBlock;
char BlockType[7] = {'o', 'i', 'j', 'l', 't', 's', 'z'};

Game::Game(){
  for(int a=0; a < 22; a++){
    for(int b=0; b <10; b++){
      GridIsOccupied[a][b] = 0;
    }
  }
}

void Game::checkLineClear(){
  int allOccupied = true;
  int linesCleared = 0;
  for(int y=0; y < 20; y++){
    allOccupied = true;
    for(int x=0; x < 10; x++){
      if(!GridIsOccupied[y][x]){
        allOccupied = false;
      }
    }
    if(allOccupied){
      lineClear(y);
      linesCleared++;
      y--;
    }
  }
  // score for line clears
  if(linesCleared == 1){
    score += 40 * level;
  }
  else if(linesCleared == 2){
    score += 100 * level;
  }
  else if(linesCleared == 3){
    score += 300 * level;
  }
  else if(linesCleared == 4){
    score += 1200 * level;
  }
}

void Game::lineClear(int line){
  for(int y=line; y < 20; y++){
    for(int x=0; x < 10; x++){
      GridIsOccupied[y][x] = GridIsOccupied[y+1][x];
    }
  }
  gameSpeed--;
}

bool Game::stopGame(){
  int y, x;
  getmaxyx(stdscr, y, x);
  
  for(int i=0; i < 10; i++){
    if(GridIsOccupied[20][i] || GridIsOccupied[21][i]){
      mvprintw(y/2-1, x/2-6,"############");
      mvprintw(y/2  , x/2-6,"# you lost #");
      mvprintw(y/2+1, x/2-6,"############");
      refresh();
      return true;
    }
  }
  return false;
}

void Game::start(){
  srand(time(0));
  
  CurrentBlock.newBlock(BlockType[rand()%7]);
  nextBlock = BlockType[rand()%7];
  
  update();
  loop();
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cin.get();
  
  return;
}

void Game::loop(){
  bool stop = false;
  struct timeval currentTime;
  
  double now, last, n=0;
  now = currentTime.tv_usec/1000;
  last = now;
  while(!stop){
    input();
    
    gettimeofday(&currentTime, NULL);
    now = currentTime.tv_usec/1000;
    
    if(last != now){
      last = now;
      n++;
      if(n > gameSpeed){
        n = 0;
        moveBlockDown();
        blockMoved = false;
        if(gameSpeed > 100){
          gameSpeed--;
        }
      }
    }
    update();
    stop = stopGame();
    std::this_thread::yield();
  }
}

void Game::input(){
  keystroke = getch();
  
  if(keystroke == KEY_LEFT){
    moveBlockLeft();
  }
  else if(keystroke == KEY_RIGHT){
    moveBlockRight();
  }
  else if(keystroke == KEY_DOWN){
    if(moveBlockDown()) score++; //soft drop + score
  }
  else if(keystroke == KEY_UP){
    while(moveBlockDown()) score += 2; // hard drop + score
    blockMoved = false;
    moveBlockDown();
  }
  else if(keystroke == 'p' || keystroke == 'P'){
    pause();
  }
  else if(keystroke == ' '){
    CurrentBlock.turnClockwise();
    blockMoved = true;
    if(GridIsOccupied[CurrentBlock.Cubes[0].y][CurrentBlock.Cubes[0].x] == true || GridIsOccupied[CurrentBlock.Cubes[1].y][CurrentBlock.Cubes[1].x] == true || GridIsOccupied[CurrentBlock.Cubes[2].y][CurrentBlock.Cubes[2].x] == true || GridIsOccupied[CurrentBlock.Cubes[3].y][CurrentBlock.Cubes[3].x] == true){
      CurrentBlock.turnCounterClockwise();
      blockMoved = false;
    }
    else if(CurrentBlock.Cubes[0].x < 0 || CurrentBlock.Cubes[0].x > 9 || CurrentBlock.Cubes[1].x < 0 || CurrentBlock.Cubes[1].x > 9 || CurrentBlock.Cubes[2].x < 0 || CurrentBlock.Cubes[2].x > 9 || CurrentBlock.Cubes[3].x < 0 || CurrentBlock.Cubes[3].x > 9){
      CurrentBlock.turnCounterClockwise();
      blockMoved = false;
    }
    else if(CurrentBlock.Cubes[0].y < 0 || CurrentBlock.Cubes[1].y < 0 || CurrentBlock.Cubes[2].y < 0 || CurrentBlock.Cubes[3].y < 0){
      CurrentBlock.turnCounterClockwise();
      blockMoved = false;
    }
  }
  /*else if(keystroke == KEY_COMMAND){
    if(!holdUsedNow){
      if(holdUsed){
        char ch;
        ch = hold;
        hold = CurrentBlock.type;
        CurrentBlock.newBlock(ch);
      }
      else{
        hold = CurrentBlock.type;
        CurrentBlock.newBlock(nextBlock);
      }
      holdUsed = true;
      holdUsedNow = true;
    }
  }*/
}

void Game::update(){
  // Note: (y-20)/2 + 19, (x-20)/2 + 2 is most bottom left block in the grid
  
  //get screen size
  int y, x;
  getmaxyx(stdscr, y, x);
  
  //clear the whole screen
  for(int a=0; a < y; a++){
    for(int b=0; b < x; b++){
      mvprintw(a, b, " ");
    }
  }
  
  //mvprintw(1, 0, "%d", CurrentBlock.type);
  
  //print upcoming block
  Block upcomingBlock;
  upcomingBlock.newBlock(nextBlock);
  mvprintw((y-20)/2 - 1, (x-20)/2 + 23, "Next block:");
  attron(A_REVERSE);
  mvprintw((y-20)/2 + 22 - upcomingBlock.Cubes[0].y, (x-20)/2 + 2 * upcomingBlock.Cubes[0].x + 18, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - upcomingBlock.Cubes[1].y, (x-20)/2 + 2 * upcomingBlock.Cubes[1].x + 18, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - upcomingBlock.Cubes[2].y, (x-20)/2 + 2 * upcomingBlock.Cubes[2].x + 18, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - upcomingBlock.Cubes[3].y, (x-20)/2 + 2 * upcomingBlock.Cubes[3].x + 18, " "); printw(" ");
  attroff(A_REVERSE);
  
  //print hold block
  /*Block holdBlock;
  holdBlock.newBlock(hold);
  mvprintw((y-20)/2 - 1, (x-20)/2 - 10, "Hold:");
  attron(A_REVERSE);
  mvprintw((y-20)/2 + 22 - holdBlock.Cubes[0].y, (x-20)/2 + 2 * holdBlock.Cubes[0].x - 16, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - holdBlock.Cubes[1].y, (x-20)/2 + 2 * holdBlock.Cubes[1].x - 16, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - holdBlock.Cubes[2].y, (x-20)/2 + 2 * holdBlock.Cubes[2].x - 16, " "); printw(" ");
  mvprintw((y-20)/2 + 22 - holdBlock.Cubes[3].y, (x-20)/2 + 2 * holdBlock.Cubes[3].x - 16, " "); printw(" ");
  attroff(A_REVERSE);*/
  
  //print boundaries for grid
  mvprintw((y-22)/2, (x-22)/2, "++++++++++++++++++++++");
  for(int row=0; row < 20; row++){
    mvprintw((y-20)/2 + row, (x-22)/2, "+                    +");
  }
  mvprintw((y-22)/2 + 21, (x-22)/2, "++++++++++++++++++++++");
  
  //Print score
  mvprintw(0, 0, "Your score is: %d", score);
  
  //Print grid
  for(int row=0; row < 20; row++){
    for(int col=0; col < 20; col++){
      if(GridIsOccupied[row][col/2]){
        attron(A_REVERSE);
      }
      mvprintw((y-20)/2 + 19 - row, (x-20)/2 + col, " ");
      attroff(A_REVERSE);
    }
  }
  
  //Print the falling block
  attron(A_REVERSE);
  for(int i=0; i < 4; i++){
    if(CurrentBlock.Cubes[i].y < 20){
      mvprintw((y-20)/2 + 19 - CurrentBlock.Cubes[i].y, (x-20)/2 + 2 * CurrentBlock.Cubes[i].x, "  ");
    }
  }
  attroff(A_REVERSE);
  
  //update the screen
  refresh();
}

void Game::lockBlock(){
  GridIsOccupied[CurrentBlock.Cubes[0].y][CurrentBlock.Cubes[0].x] = true;
  GridIsOccupied[CurrentBlock.Cubes[1].y][CurrentBlock.Cubes[1].x] = true;
  GridIsOccupied[CurrentBlock.Cubes[2].y][CurrentBlock.Cubes[2].x] = true;
  GridIsOccupied[CurrentBlock.Cubes[3].y][CurrentBlock.Cubes[3].x] = true;
  checkLineClear();
  CurrentBlock.newBlock(nextBlock);
  nextBlock = BlockType[rand()%7];
  holdUsedNow = false;
}

void Game::pause(){
  bool stop = false;
  int y, x;
  getmaxyx(stdscr, y, x);
  while(!stop){
    getmaxyx(stdscr, y, x);
    update();
    mvprintw((y-20)/2 + 9, (x-20)/2 + 1, "Press P to resume.");
    refresh();
    keystroke = std::cin.get();
    if(keystroke == 'p' || keystroke == 'P'){
      stop = true;
    }
  }
}

bool Game::moveBlockDown(){
  if(CurrentBlock.Cubes[0].y-1 < 0 || CurrentBlock.Cubes[1].y-1 < 0 || CurrentBlock.Cubes[2].y-1 < 0 || CurrentBlock.Cubes[3].y-1 < 0){
    if(!blockMoved){
      lockBlock();
    }
    return false;
  }
  else if(GridIsOccupied[CurrentBlock.Cubes[0].y-1][CurrentBlock.Cubes[0].x] || GridIsOccupied[CurrentBlock.Cubes[1].y-1][CurrentBlock.Cubes[1].x] || GridIsOccupied[CurrentBlock.Cubes[2].y-1][CurrentBlock.Cubes[2].x] || GridIsOccupied[CurrentBlock.Cubes[3].y-1][CurrentBlock.Cubes[3].x]){
    if(!blockMoved){
      lockBlock();
    }
    return false;
  }
  CurrentBlock.moveDown();
  blockMoved = true;
  return true;
}

void Game::moveBlockLeft(){
  if(CurrentBlock.Cubes[0].x-1 < 0 || CurrentBlock.Cubes[1].x-1 < 0 || CurrentBlock.Cubes[2].x-1 < 0 || CurrentBlock.Cubes[3].x-1 < 0){
    return;
  }
  else if(GridIsOccupied[CurrentBlock.Cubes[0].y][CurrentBlock.Cubes[0].x-1] || GridIsOccupied[CurrentBlock.Cubes[1].y][CurrentBlock.Cubes[1].x-1] || GridIsOccupied[CurrentBlock.Cubes[2].y][CurrentBlock.Cubes[2].x-1] || GridIsOccupied[CurrentBlock.Cubes[3].y][CurrentBlock.Cubes[3].x-1]){
    return;
  }
  else{
    CurrentBlock.moveLeft();
    blockMoved = true;
    return;
  }
}

void Game::moveBlockRight(){
  if(GridIsOccupied[CurrentBlock.Cubes[0].y][CurrentBlock.Cubes[0].x+1] || GridIsOccupied[CurrentBlock.Cubes[1].y][CurrentBlock.Cubes[1].x+1] || GridIsOccupied[CurrentBlock.Cubes[2].y][CurrentBlock.Cubes[2].x+1] || GridIsOccupied[CurrentBlock.Cubes[3].y][CurrentBlock.Cubes[3].x+1]){
    return;
  }
  else if(CurrentBlock.Cubes[0].x+1 > 9 || CurrentBlock.Cubes[1].x+1 > 9 || CurrentBlock.Cubes[2].x+1 > 9 || CurrentBlock.Cubes[3].x+1 > 9){
    return;
  }
  else{
    CurrentBlock.moveRight();
    blockMoved = true;
    return;
  }
}