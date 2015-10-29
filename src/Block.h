#ifndef BLOCK_H
#define BLOCK_H

class Block{
  public:
    struct CubeStruct{
      int y, x;
    };
    float pivotY, pivotX;
    CubeStruct Cubes[3];
    char type;
    
    void newBlock(char);
    void turnClockwise();
    void turnCounterClockwise();
    void moveDown();
    void moveLeft();
    void moveRight();
};

#endif