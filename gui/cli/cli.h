#ifndef CLI_H_
#define CLI_H_

#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"

void initGui();
void printGame(Game *game);
void printField(Game *game);
void printFigure(Game *game);
void printNextFigure(Game *game);
void printInfo(GameInfo *gameInfo);
void getActions(Game *game);
UserAction check_symbol(char ch);

#endif