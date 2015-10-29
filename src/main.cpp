#include <ncurses.h>
#include "Game.h"

int main(){
	initscr();
	//cbreak();
	//raw();
  noecho();
	curs_set(0);
	nodelay(stdscr,TRUE);
  keypad(stdscr, true);
  
	Game Tetris;
	Tetris.start();
	
	endwin();
	return 0;
}