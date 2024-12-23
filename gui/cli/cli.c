#include "cli.h"

#include <time.h>
#include <unistd.h>
/** @file */

/**
 * @brief Инициализация NCURSES.
 *
 * @return статус выполнения функции.
 */
void initGui() {
  initscr();
  curs_set(0);
  start_color();
  init_color(10, 500, 500, 500);
  init_pair(1, COLOR_BLACK, 10);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, 10, COLOR_BLACK);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
}

/**
 * @brief Отображает все элементы игры
 *
 * @param game Указатель на структуру Game, содержащую данные о текущей игре.
 * @param sp_start Время начала текущего шага.
 * @param sp_end Время конца текущего шага.
 */
void printGame(Game *game) {
  printField(game);
  printFigure(game);
  printNextFigure(game);
  printInfo(game->gameInfo);

  timeout(TICKS);

  refresh();
}

/**
 * @brief Отображает игровое поле.
 *
 * @param game Указатель на структуру Game, содержащую данные о поле.
 */
void printField(Game *game) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int sym = game->field->blocks[i][j].block ? 2 : 1;
      attron(COLOR_PAIR(sym));
      mvaddch(i + 3, j * 2 + 2, ' ');
      mvaddch(i + 3, j * 2 + 3, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
}

/**
 * @brief Отображает текущую фигуру на игровом поле.
 *
 * Использует координаты фигуры и ее блоки для вывода на экран.
 *
 * @param game Указатель на структуру Game, содержащую текущую фигуру.
 */
void printFigure(Game *game) {
  Figure *figure = game->figure;
  for (int i = 0; i < FIGURE_HEIGHT; ++i) {
    for (int j = 0; j < FIGURE_WIDTH; ++j) {
      if (figure->blocks[i][j].block != 0) {
        attron(COLOR_PAIR(2));
        mvaddch(i + 3 + figure->y, j * 2 + 2 + figure->x * 2, ' ');
        mvaddch(i + 3 + figure->y, j * 2 + 3 + figure->x * 2, ' ');
        attroff(COLOR_PAIR(2));
      }
    }
  }
}

/**
 * @brief Отображает следующую фигуру в отдельной области экрана.
 *
 * Выводит предварительный просмотр следующей фигуры, чтобы игрок мог
 * планировать действия.
 *
 * @param game Указатель на структуру Game, содержащую данные о следующей
 * фигуре.
 */
void printNextFigure(Game *game) {
  for (int i = 0; i < FIGURE_HEIGHT; i++) {
    for (int j = 0; j < FIGURE_WIDTH; j++) {
      int num =
          game->figurest->blocks[game->gameInfo->nextID][i * FIGURE_WIDTH + j]
                  .block
              ? 2
              : 3;
      attron(COLOR_PAIR(num));
      mvaddch(i + 5, j * 2 + 28, ' ');
      mvaddch(i + 5, j * 2 + 29, ' ');
      attroff(COLOR_PAIR(num));
    }
  }
}

/**
 * @brief Отображает информацию о текущем состоянии игры.
 *
 * Выводит уровень, скорость, счет, максимальный результат.
 *
 * @param gameInfo Указатель на структуру GameInfo, содержащую данные об игре.
 */
void printInfo(GameInfo *gameInfo) {
  attron(COLOR_PAIR(4));
  mvwprintw(stdscr, 1, 10, "TETRIS");
  attroff(COLOR_PAIR(4));

  attron(COLOR_PAIR(3));
  mvwprintw(stdscr, 3, 26, "Next figure:");
  mvwprintw(stdscr, 11, 26, "Lvl: %d", gameInfo->level);
  mvwprintw(stdscr, 13, 26, "Speed: %d", gameInfo->speed);
  mvwprintw(stdscr, 15, 26, "Score: %d", gameInfo->score);
  mvwprintw(stdscr, 17, 26, "nextID: %d", gameInfo->nextID);
  clrtoeol();
  mvwprintw(stdscr, 17, 26, "High score: %d", gameInfo->high_score);
  mvwprintw(stdscr, 19, 26, "nextID: %d", gameInfo->nextID);

  if (gameInfo->pause) mvwprintw(stdscr, 12, 2, "Press ENTER to play.");
  if (gameInfo->state == GameOver)
    mvwprintw(stdscr, 12, 2, "      GameOver      ");
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(4));
  mvwprintw(stdscr, 3, 45, "Start: 'Enter'");
  mvwprintw(stdscr, 4, 45, "Pause: 'p'");
  mvwprintw(stdscr, 5, 45, "Exit: 'q'");
  mvwprintw(stdscr, 6, 45, "Arrows to move: 'a' 'd'");
  mvwprintw(stdscr, 7, 45, "Space to rotate");
  mvwprintw(stdscr, 8, 45, "Arrow down to plant: 's'");
  mvwprintw(stdscr, 10, 45, "%d", gameInfo->state);
  attroff(COLOR_PAIR(4));
}

/**
 * @brief Считывает действия игрока из ввода.
 *
 * Преобразует нажатия клавиш в действия игрока, такие как поворот, движение или
 * пауза.
 *
 * @param game Указатель на структуру Game, в которой обновляются действия
 * игрока.
 */
void getActions(Game *game) {
  char ch = getch();
  switch (ch) {
    case ' ':
      game->player->action = ROTATE;
      break;
    case 66:
      game->player->action = DOWN;
      break;
    case 67:
      game->player->action = RIGHT;
      break;
    case 68:
      game->player->action = LEFT;
      break;
    case '\n':
      game->player->action = START;
      break;
    case 'p':
      game->player->action = PAUSE;
      break;
    case 'q':
      game->player->action = TERMINATE;
      break;
    default:
      game->player->action = ACTION;
      break;
  }
}