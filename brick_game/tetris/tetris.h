#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_WIDTH 10  /*!< Ширина игрового поля */
#define FIELD_HEIGHT 20 /*!< Высота игрового поля */
#define FIGURE_WIDTH 5  /*!< Ширина фигуры */
#define FIGURE_HEIGHT 5 /*!< Высота фигуры */
#define FIGURES_COUNT 7 /*!< Общее количество фигур */
#define TICKS 30 /*!< Количество тиков в одной игровой итерации */

typedef enum GameState {
  Start,   ///< Инициализация игры
  Pause,   ///< Игра приостановлена
  Spawn,   ///< Генерирование следующей фигуры
  Moving,  ///< Движение влево, вправо, вниз
  Collision,  ///< Столкновение фигуры с другими блоками
  GameOver,   ///< Конец игры
  Quit        ///< Выход из игры
} GameState;  ///< Тип, представляющий состояния игры "Тетрис"

typedef enum UserAction {
  START,      ///< Инициализация игры
  PAUSE,      ///< Пауза
  TERMINATE,  ///< Конец игры или выход
  LEFT,       ///< Перемещение влево
  RIGHT,      ///< Перемещение вправо
  DOWN,       ///< Перемещение вниз
  ROTATE,     ///< Поворот фигуры
  ACTION      ///< Действие в игре
} UserAction;

/**
 * @struct Block
 * @brief Структура, представляющая блок.
 *
 * Эта структура описывает один блок, используемый в фигурах и игровом поле.
 */
typedef struct Block {
  int block;  ///< 0 - пустой блок, 1 - заполненный блок
} Block;

/**
 * @struct Field
 * @brief Структура, представляющая игровое поле.
 *
 * Эта структура содержит массив блоков, представляющих игровое поле,
 * на котором располагаются фигуры.
 */
typedef struct Field {
  Block **blocks;  ///< Двумерный массив блоков, представляющий поле
} Field;

/**
 * @struct Figure
 * @brief Структура, представляющая фигуру.
 *
 * Эта структура содержит координаты фигуры и массив блоков, которые
 * образуют фигуру.
 */
typedef struct Figure {
  int x;  ///< Координата по горизонтали (сдвиг)
  int y;  ///< Координата по вертикали (высота)
  Block **blocks;  ///< Двумерный массив блоков, представляющий фигуру
} Figure;

/**
 * @struct FiguresT
 * @brief Структура, представляющая все фигуры.
 *
 * Эта структура содержит массив блоков для всех фигур, доступных в игре.
 */
typedef struct FiguresT {
  Block **blocks;  ///< Двумерный массив блоков для всех фигур
} FiguresT;

/**
 * @struct Player
 * @brief Структура, представляющая игрока.
 *
 * Эта структура содержит действия, которые может выполнить игрок во время игры.
 */
typedef struct Player {
  UserAction
      action;  ///< Действие игрока, например, перемещение или поворот фигуры
} Player;

/**
 * @struct GameInfo
 * @brief Структура, содержащая информацию об игре.
 *
 * Эта структура хранит информацию о текущем состоянии игры, включая
 * идентификатор следующей фигуры, счёт, уровень и скорость игры.
 */
typedef struct GameInfo {
  int nextID;  ///< Идентификатор следующей фигуры
  int score;   ///< Текущий счёт игрока
  int high_score;  ///< Рекордный счёт
  int level;       ///< Уровень сложности
  int speed;       ///< Скорость игры
  int pause;  ///< Флаг паузы (0 - не приостановлено, 1 - приостановлено)
  int ticks_left;  ///< Остаток тиков до следующего действия
  int ticks;        ///< Общее количество тиков
  GameState state;  ///< Текущее состояние игры
} GameInfo;

/**
 * @struct Game
 * @brief Структура, представляющая состояние игры.
 *
 * Эта структура объединяет всю необходимую информацию для управления
 * игровым процессом, включая информацию об игре, поле, текущую фигуру
 * и игрока.
 */
typedef struct Game {
  GameInfo *gameInfo;  ///< Указатель на информацию об игре
  Field *field;        ///< Указатель на игровое поле
  Figure *figure;  ///< Указатель на текущую фигуру
  FiguresT *figurest;  ///< Указатель на все доступные фигуры
  Player *player;  ///< Указатель на игрока
} Game;  ///< Тип, представляющий состояние игры "Тетрис"

// init object
Game *initGame();
GameInfo *createGameInfo();
Field *createField();
Figure *createFigure();
FiguresT *createFiguresT();
Player *createPlayer();
int **createNextBlock(Game *game);

// free object
void freeGame(Game *game);
void freeGameInfo(GameInfo *gameInfo);
void freeField(Field *field);
void freeFigure(Figure *figure);
void freeFiguresT(FiguresT *figureT);
void freeNextBlock(int **next);

// highScore
void saveHighScore(int high_score);
int loadHighScore();

// move figure
void upFigure(Figure *figure);
void downFigure(Figure *figure);
void leftFigure(Figure *figure);
void rightFigure(Figure *figure);
Figure *rotationFigure(Game *game);

// logic
bool inField(int fx, int fy);
void dropNewFigure(Game *game);
void updateCurrentState(Game *game);
void calculate(Game *game);
void calcOne(Game *game);
bool collision(Game *game);
int eraseLines(Field *field);
bool lineFilled(int i, Field *field);
void dropLine(int i, Field *field);
void pause_game(Game *game);
void rotate(Game *game);
void up(Game *game);
void down(Game *game);
void left(Game *game);
void right(Game *game);
void plantFigure(Game *game);
void countScore(Game *game);

#endif