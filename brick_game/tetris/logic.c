/**
 * @file game.c
 * @brief Функции для управления игровым процессом Tetris
 *
 * Этот файл содержит функции, отвечающие за перемещение фигур,
 * обработку столкновений, подсчет очков и другие аспекты игрового процесса
 */
#include "tetris.h"

/**
 * @brief Проверяет, находится ли координата фигуры в пределах игрового поля.
 * @param fx Координата X фигуры.
 * @param fy Координата Y фигуры.
 * @return true, если фигура находится в пределах поля, иначе false.
 */
bool inField(int fx, int fy) {
  return fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT;
}

/**
 * @brief Перемещает фигуру вверх.
 */
void upFigure(Figure *figure) { figure->y--; }

/**
 * @brief Перемещает фигуру вниз.
 */
void downFigure(Figure *figure) { figure->y++; }

/**
 * @brief Перемещает фигуру влево.
 */
void leftFigure(Figure *figure) { figure->x--; }

/**
 * @brief Перемещает фигуру вправо.
 */
void rightFigure(Figure *figure) { figure->x++; }

/**
 * @brief Создает новую фигуру и устанавливает ее начальные координаты.
 * @param game Указатель на объект игры.
 */
void dropNewFigure(Game *game) {
  Figure *figure = createFigure();
  figure->x = FIELD_WIDTH / 2 - FIGURE_WIDTH / 2;
  figure->y = 0;

  for (int i = 0; i < FIGURE_HEIGHT; ++i) {
    for (int j = 0; j < FIGURE_WIDTH; ++j)
      figure->blocks[i][j].block =
          game->figurest->blocks[game->gameInfo->nextID][i * FIGURE_WIDTH + j]
              .block;
  }
  
  game->figure = figure;
  game->gameInfo->nextID = rand() % FIGURES_COUNT;
}

/**
 * @brief Обрабатывает игровую логику, включая управление фигурой и состояния
 * игры.
 * @param game Указатель на объект игры.
 */
void calculate(Game *game) {
  if (game->gameInfo->ticks_left <= 0 && game->gameInfo->state != Pause &&
      game->gameInfo->state != Start && game->gameInfo->state != GameOver)
    calcOne(game);
  if (game->gameInfo->state != Quit && game->gameInfo->state != GameOver) {
    switch (game->player->action) {
      case PAUSE:
        pause_game(game);
        break;
      case ROTATE:
        rotate(game);
        break;
      case DOWN:
        down(game);
        break;
      case LEFT:
        left(game);
        break;
      case RIGHT:
        right(game);
        break;
      case TERMINATE:
        game->gameInfo->state = Quit;
        break;
      case Start:
        game->gameInfo->pause = 0;
        game->gameInfo->state = Moving;
        break;
      default:
        break;
    }
    game->gameInfo->ticks_left--;
  }
}

/**
 * @brief Выполняет один такт игры, перемещая фигуру вниз и обрабатывая
 * столкновения.
 * @param game Указатель на объект игры.
 */
void calcOne(Game *game) {
  game->gameInfo->ticks_left = game->gameInfo->ticks;
  game->gameInfo->state = Moving;
  downFigure(game->figure);
  if (collision(game)) {
    upFigure(game->figure);
    plantFigure(game);
    countScore(game);
    freeFigure(game->figure);
    dropNewFigure(game);
    game->gameInfo->state = Spawn;
    if (collision(game)) {
      game->gameInfo->state = GameOver;
    }
  }
}

/**
 * @brief Проверяет наличие столкновений фигуры с другими объектами или
 * границами поля.
 * @param game Указатель на объект игры.
 * @return true, если произошло столкновение, иначе false.
 */
bool collision(Game *game) {
  Field *field = game->field;
  Figure *figure = game->figure;

  for (int i = 0; i < FIGURE_HEIGHT && game->gameInfo->state != Collision; ++i)
    for (int j = 0; j < FIGURE_WIDTH && game->gameInfo->state != Collision;
         ++j) {
      if (figure->blocks[i][j].block) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (!inField(fx, fy))
          game->gameInfo->state = Collision;
        else if (field->blocks[fy][fx].block)
          game->gameInfo->state = Collision;
      }
    }
  return game->gameInfo->state == Collision;
}

/**
 * @brief Фиксирует фигуру на игровом поле.
 * @param game Указатель на объект игры.
 */
void plantFigure(Game *game) {
  for (int i = 0; i < FIGURE_HEIGHT; i++)
    for (int j = 0; j < FIGURE_WIDTH; j++)
      if (game->figure->blocks[i][j].block) {
        int fx = game->figure->x + j;
        int fy = game->figure->y + i;
        if (inField(fx, fy)) {
          game->field->blocks[fy][fx].block = 1;
        }
      }
}

/**
 * @brief Удаляет заполненные линии из игрового поля и возвращает количество
 * удаленных линий.
 * @param field Указатель на игровое поле.
 * @return Количество удаленных линий.
 */
int eraseLines(Field *field) {
  int count = 0;
  for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
    while (lineFilled(i, field)) {
      dropLine(i, field);
      count++;
    }
  }
  return count;
}

/**
 * @brief Проверяет, заполнена ли линия на игровом поле.
 * @param i Индекс линии.
 * @param field Указатель на игровое поле.
 * @return true, если линия заполнена, иначе false.
 */
bool lineFilled(int i, Field *field) {
  bool flag = true;
  for (int j = 0; j < FIELD_WIDTH && flag; ++j)
    if (!field->blocks[i][j].block) flag = false;
  return flag;
}

/**
 * @brief Смещает линии вниз после удаления заполненной линии.
 * @param i Индекс линии.
 * @param field Указатель на игровое поле.
 */
void dropLine(int i, Field *field) {
  if (i == 0)
    for (int j = 0; j < FIELD_WIDTH; j++) field->blocks[i][j].block = 0;
  else {
    for (int k = i; k > 0; k--)
      for (int j = 0; j < FIELD_WIDTH; j++)
        field->blocks[k][j].block = field->blocks[k - 1][j].block;
  }
}

/**
 * @brief Меняет состояние паузы игры.
 * @param game Указатель на объект игры.
 */
void pause_game(Game *game) {
  if (game->gameInfo->pause) {
    game->gameInfo->pause = 0;
    game->gameInfo->state = Moving;
  } else {
    game->gameInfo->pause = 1;
    game->gameInfo->state = Pause;
  }
}

/**
 * @brief Перемещает фигуру вниз, если игра не на паузе и нет столкновений
 * @param game Указатель на объект игры.
 */
void down(Game *game) {
  if (!game->gameInfo->pause) {
    downFigure(game->figure);
    if (collision(game)) upFigure(game->figure);
  }
}

/**
 * @brief Перемещает фигуру влево, если игра не на паузе и нет столкновений
 * @param game Указатель на объект игры.
 */
void left(Game *game) {
  if (!game->gameInfo->pause) {
    leftFigure(game->figure);
    if (collision(game)) rightFigure(game->figure);
  }
}

/**
 * @brief Перемещает фигуру вправо, если игра не на паузе и нет столкновений
 * @param game Указатель на объект игры.
 */
void right(Game *game) {
  if (!game->gameInfo->pause) {
    rightFigure(game->figure);
    if (collision(game)) leftFigure(game->figure);
  }
}

/**
 * @brief Поворачивает фигуру, если игра не на паузе и нет столкновений
 * @param game Указатель на объект игры.
 */
void rotate(Game *game) {
  if (!game->gameInfo->pause) {
    Figure *pastFigure = game->figure;
    game->figure = rotationFigure(game);
    if (collision(game)) {
      freeFigure(game->figure);
      game->figure = pastFigure;
    } else {
      freeFigure(pastFigure);
    }
  }
}

/**
 * @brief Создает новую фигуру, повернутую на 90 градусов.
 * @param game Указатель на объект игры.
 * @return Указатель на новую повернутую фигуру.
 */
Figure *rotationFigure(Game *game) {
  Figure *figure = createFigure(game);
  figure->x = game->figure->x;
  figure->y = game->figure->y;

  for (int i = 0; i < FIGURE_HEIGHT; ++i)
    for (int j = 0; j < FIGURE_WIDTH; ++j)
      figure->blocks[i][j].block =
          game->figure->blocks[j][FIGURE_WIDTH - 1 - i].block;

  return figure;
}

/**
 * @brief Подсчитывает очки за удаленные линии и обновляет уровень.
 * @param game Указатель на объект игры.
 */
void countScore(Game *game) {
  int erased_lines = eraseLines(game->field);
  switch (erased_lines) {
    case 0:
      break;
    case 1:
      game->gameInfo->score += 100;
      break;
    case 2:
      game->gameInfo->score += 300;
      break;
    case 3:
      game->gameInfo->score += 700;
      break;
    default:
      game->gameInfo->score += 1500;
      break;
  }
  if (game->gameInfo->score > game->gameInfo->high_score) {
    game->gameInfo->high_score = game->gameInfo->score;
    saveHighScore(game->gameInfo->high_score);
  }

  int new_level = game->gameInfo->score / 600 + 1;
  if (new_level > game->gameInfo->level && new_level <= 10) {
    game->gameInfo->level = new_level;
    game->gameInfo->speed = new_level;
  }
}
