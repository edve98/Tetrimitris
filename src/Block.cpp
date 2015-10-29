#include "Block.h"
#include <iostream>

void Block::newBlock(char blockType){
  type = blockType;
  if(type == 'o'){
    Cubes[0].y = 21;
    Cubes[0].x = 4;
    Cubes[1].y = 21;
    Cubes[1].x = 5;
    Cubes[2].y = 20;
    Cubes[2].x = 4;
    Cubes[3].y = 20;
    Cubes[3].x = 5;
    pivotY = 20.5;
    pivotX = 4.5;
  }
  else if(type == 'i'){
    Cubes[0].y = 20;
    Cubes[0].x = 3;
    Cubes[1].y = 20;
    Cubes[1].x = 4;
    Cubes[2].y = 20;
    Cubes[2].x = 5;
    Cubes[3].y = 20;
    Cubes[3].x = 6;
    pivotY = 19.5;
    pivotX = 4.5;
  }
  else if(type == 'j'){
    Cubes[0].y = 21;
    Cubes[0].x = 3;
    Cubes[1].y = 20;
    Cubes[1].x = 3;
    Cubes[2].y = 20;
    Cubes[2].x = 4;
    Cubes[3].y = 20;
    Cubes[3].x = 5;
    pivotY = 20;
    pivotX = 4;
  }
  else if(type == 'l'){
    Cubes[0].y = 21;
    Cubes[0].x = 5;
    Cubes[1].y = 20;
    Cubes[1].x = 3;
    Cubes[2].y = 20;
    Cubes[2].x = 4;
    Cubes[3].y = 20;
    Cubes[3].x = 5;
    pivotY = 20;
    pivotX = 4;
  }
  else if(type == 't'){
    Cubes[0].y = 21;
    Cubes[0].x = 4;
    Cubes[1].y = 20;
    Cubes[1].x = 3;
    Cubes[2].y = 20;
    Cubes[2].x = 4;
    Cubes[3].y = 20;
    Cubes[3].x = 5;
    pivotY = 20;
    pivotX = 4;
  }
  else if(type == 's'){
    Cubes[0].y = 21;
    Cubes[0].x = 4;
    Cubes[1].y = 21;
    Cubes[1].x = 5;
    Cubes[2].y = 20;
    Cubes[2].x = 3;
    Cubes[3].y = 20;
    Cubes[3].x = 4;
    pivotY = 20;
    pivotX = 4;
  }
  else if(type == 'z'){
    Cubes[0].y = 21;
    Cubes[0].x = 3;
    Cubes[1].y = 21;
    Cubes[1].x = 4;
    Cubes[2].y = 20;
    Cubes[2].x = 4;
    Cubes[3].y = 20;
    Cubes[3].x = 5;
    pivotY = 20;
    pivotX = 4;
  }
}

void Block::turnCounterClockwise(){
  CubeStruct cubes[4];
  
  cubes[0].y = pivotY - pivotX + Cubes[0].x;
  cubes[0].x = pivotX + pivotY - Cubes[0].y;
  cubes[1].y = pivotY - pivotX + Cubes[1].x;
  cubes[1].x = pivotX + pivotY - Cubes[1].y;
  cubes[2].y = pivotY - pivotX + Cubes[2].x;
  cubes[2].x = pivotX + pivotY - Cubes[2].y;
  cubes[3].y = pivotY - pivotX + Cubes[3].x;
  cubes[3].x = pivotX + pivotY - Cubes[3].y;
  
  Cubes[0].y = cubes[0].y;
  Cubes[0].x = cubes[0].x;
  Cubes[1].y = cubes[1].y;
  Cubes[1].x = cubes[1].x;
  Cubes[2].y = cubes[2].y;
  Cubes[2].x = cubes[2].x;
  Cubes[3].y = cubes[3].y;
  Cubes[3].x = cubes[3].x;
}

void Block::turnClockwise(){
  turnCounterClockwise();
  turnCounterClockwise();
  turnCounterClockwise();
}

void Block::moveDown(){
  Cubes[0].y--;
  Cubes[1].y--;
  Cubes[2].y--;
  Cubes[3].y--;
  pivotY--;
}

void Block::moveLeft(){
  Cubes[0].x--;
  Cubes[1].x--;
  Cubes[2].x--;
  Cubes[3].x--;
  pivotX--;
}

void Block::moveRight(){
  Cubes[0].x++;
  Cubes[1].x++;
  Cubes[2].x++;
  Cubes[3].x++;
  pivotX++;
}