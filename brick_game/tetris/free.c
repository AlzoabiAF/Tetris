/**
 * @file free.c
 * @brief Файл для освобождения ресурсов в игре Tetris.
 *
 * Этот файл содержит функции для освобождения памяти,
 * выделенной для различных структур данных игры, таких как
 * фигуры, игровое поле, информация об игре и игроках.
 * Используется для предотвращения утечек памяти при завершении игры.
 *
 * @author [Ваше Имя]
 * @date [Дата создания]
 */

#include "tetris.h"

/**
 * @brief Освобождает память, занятую объектом игры.
 * @param game Указатель на объект игры, который необходимо освободить.
 */
void freeGame(Game *game) {
  if (game) {
    freeFigure(game->figure);
    freeGameInfo(game->gameInfo);
    freeField(game->field);
    freeFiguresT(game->figurest);
    free(game->player);
    free(game);
  }
}

/**
 * @brief Освобождает память, занятую объектом GameInfo.
 * @param gameInfo Указатель на объект GameInfo, который необходимо освободить.
 */
void freeGameInfo(GameInfo *gameInfo) {
  if (gameInfo) free(gameInfo);
}

/**
 * @brief Освобождает память, занятую игровым полем.
 * @param field Указатель на объект Field, который необходимо освободить.
 */
void freeField(Field *field) {
  if (field) {
    for (int i = 0; i < FIELD_HEIGHT; i++) free(field->blocks[i]);
    free(field->blocks);
    free(field);
  }
}

/**
 * @brief Освобождает память, занятую фигурой.
 * @param figure Указатель на объект Figure, который необходимо освободить.
 */
void freeFigure(Figure *figure) {
  if (figure) {
    if (figure->blocks) {
      for (int i = 0; i < FIGURE_HEIGHT; i++) {
        if (figure->blocks[i]) {
          free(figure->blocks[i]);
        }
      }
      free(figure->blocks);
    }
    free(figure);
  }
}

/**
 * @brief Освобождает память, занятую набором фигур.
 * @param figureT Указатель на объект FiguresT, который необходимо освободить.
 */
void freeFiguresT(FiguresT *figureT) {
  if (figureT) {
    if (figureT->blocks) {
      free(figureT->blocks);
    }
    free(figureT);
  } 
}
