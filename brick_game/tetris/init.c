/**
 * @file init.c
 * @brief Файл инициализации игры Tetris.
 *
 * Этот файл содержит функции для создания и инициализации объектов,
 * таких как игровое поле, фигуры и информация об игре. Эти функции
 * используются для подготовки игры к началу и обновления её состояния
 * во время игры.
 */

#include "figures.h"
#include "tetris.h"

/**
 * @brief Инициализирует объект игры и возвращает указатель на него.
 * @return Указатель на инициализированный объект игры.
 */
Game *initGame() {
  Game *game = (Game *)malloc(sizeof(Game));

  game->gameInfo = createGameInfo();
  game->field = createField();
  game->figure=NULL;
  game->figurest = createFiguresT();
  game->player = createPlayer();
  game->gameInfo->nextID = rand() % FIGURES_COUNT;

  dropNewFigure(game);
  
  return game;
}

/**
 * @brief Создает объект GameInfo и инициализирует его поля.
 * @return Указатель на инициализированный объект GameInfo.
 */
GameInfo *createGameInfo() {
  GameInfo *gameInfo = (GameInfo *)malloc(sizeof(GameInfo));
  gameInfo->score = 0;
  gameInfo->high_score = loadHighScore();
  gameInfo->ticks = 30;
  gameInfo->ticks_left = 30;
  gameInfo->speed = 1;
  gameInfo->level = 1;
  gameInfo->state = Start;
  gameInfo->pause = 1;
  gameInfo->nextID = rand() % FIGURES_COUNT;

  return gameInfo;
}

/**
 * @brief Создает игровое поле и инициализирует его блоки.
 * @return Указатель на инициализированное игровое поле.
 */
Field *createField() {
  Field *field = (Field *)malloc(sizeof(Field));
  field->blocks = (Block **)malloc(sizeof(Block *) * FIELD_HEIGHT);
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    field->blocks[i] = (Block *)malloc(sizeof(Block) * FIELD_WIDTH);
    for (int j = 0; j < FIELD_WIDTH; j++) {
      field->blocks[i][j].block = 0;
    }
  }

  return field;
}

/**
 * @brief Создает фигуру и инициализирует её поля.
 * @return Указатель на инициализированную фигуру.
 */
Figure *createFigure() {
  Figure *figure = (Figure *)malloc(sizeof(Figure));
  figure->x = 0;
  figure->y = 0;
  figure->blocks = (Block **)malloc(sizeof(Block *) * FIGURE_HEIGHT);
  for (int i = 0; i < FIGURE_HEIGHT; i++) {
    figure->blocks[i] = (Block *)malloc(sizeof(Block) * FIGURE_WIDTH);
  }
  return figure;
}

/**
 * @brief Создает структуру FiguresT и инициализирует её фигурами.
 * @return Указатель на инициализированный объект FiguresT.
 */
FiguresT *createFiguresT() {
  FiguresT *figurest = (FiguresT *)malloc(sizeof(FiguresT));

  Block **templates = malloc(FIGURES_COUNT * sizeof(Block *));
  templates[0] = &iFigure[0][0];
  templates[1] = &oFigure[0][0];
  templates[2] = &tFigure[0][0];
  templates[3] = &sFigure[0][0];
  templates[4] = &zFigure[0][0];
  templates[5] = &jFigure[0][0];
  templates[6] = &lFigure[0][0];

  figurest->blocks = templates;

  return figurest;
}

/**
 * @brief Создает объект Player и инициализирует его поля.
 * @return Указатель на инициализированный объект Player.
 */
Player *createPlayer() {
  Player *player = (Player *)malloc(sizeof(Player));
  player->action = START;
  return player;
}

/**
 * @brief Сохраняет высокий счёт в файл.
 * @param high_score Высокий счёт для сохранения.
 */
void saveHighScore(int high_score) {
  FILE *file = fopen("high_score.dat", "w");
  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

/**
 * @brief Загружает высокий счёт из файла.
 * @return Загруженный высокий счёт.
 */
int loadHighScore() {
  int high_score = 0;
  FILE *file = fopen("high_score.dat", "r");
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}
