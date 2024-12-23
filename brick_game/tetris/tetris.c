/**
 * @file tetris.c
 * @brief Файл запуска игры Tetris
 *
 * Он отвечает за инициализацию графического интерфейса, управление игровым
 * процессом и завершение игры
 */

#include "tetris.h"

#include "../../gui/cli/cli.h"

/**
 * @brief Запуск Tetris
 *
 * Эта функция инициализирует графический интерфейс и основной игровой процесс.
 * Она запускает цикл игры, обрабатывающий действия игрока и обновляющий
 * состояние игры, пока игрок не решит выйти
 *
 * @return Возвращает 0 при успешном завершении
 */
int main() {
  srand((unsigned int)time(NULL));
  initGui();
  Game *game = initGame();

  while (game->gameInfo->state != Quit) {
    getActions(game);

    if (game->gameInfo->state != GameOver) {
      calculate(game);
      printGame(game);
    } else {
      if (game->player->action == START) {
        freeGame(game);
        game = initGame();
      }
    }
  }

  freeGame(game);
  endwin();

  return 0;
}