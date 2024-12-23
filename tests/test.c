#include "test.h"

UserAction check_symbol(char ch);

START_TEST(init_1) {
  Game *game = initGame();
  int score = game->gameInfo->score;

  ck_assert_int_eq(score, 0);

  freeGame(game);
}
END_TEST

START_TEST(init_2) {
  Game *game = initGame();
  int level = game->gameInfo->level;

  ck_assert_int_eq(level, 1);

  freeGame(game);
}
END_TEST

START_TEST(init_3) {
  Game *game = initGame();
  int pause = game->gameInfo->pause;
  ck_assert_int_eq(pause, 1);

  freeGame(game);
}
END_TEST

START_TEST(status_1) {
  Game *game = initGame();

  game->player->action = check_symbol('q');
  calculate(game);
  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Quit);

  freeGame(game);
}
END_TEST

START_TEST(status_2) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Moving);

  freeGame(game);
}
END_TEST

START_TEST(terminate) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol('q');
  calculate(game);
  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Quit);

  freeGame(game);
}
END_TEST

START_TEST(pause_1) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol('p');
  calculate(game);
  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Pause);

  freeGame(game);
}
END_TEST

START_TEST(pause_2) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol('p');
  calculate(game);
  game->player->action = check_symbol('p');
  calculate(game);

  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Moving);

  freeGame(game);
}
END_TEST

START_TEST(moving_left) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol(68);  // left
  calculate(game);
  GameState status = game->gameInfo->state;
  int x = game->figure->x;
  int y = game->figure->y;

  ck_assert_int_eq(status, Moving);
  ck_assert_int_eq(x, 2);
  ck_assert_int_eq(y, 0);

  freeGame(game);
}
END_TEST

START_TEST(moving_right) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol(67);  // left
  calculate(game);
  GameState status = game->gameInfo->state;
  int x = game->figure->x;
  int y = game->figure->y;

  ck_assert_int_eq(status, Moving);
  ck_assert_int_eq(x, 4);
  ck_assert_int_eq(y, 0);

  freeGame(game);
}
END_TEST

START_TEST(moving_down) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol(66);  // left
  calculate(game);
  GameState status = game->gameInfo->state;
  int x = game->figure->x;
  int y = game->figure->y;

  ck_assert_int_eq(status, Moving);
  ck_assert_int_eq(x, 3);
  ck_assert_int_eq(y, 1);

  freeGame(game);
}
END_TEST

START_TEST(moving_action) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->gameInfo->ticks_left = 0;
  calculate(game);
  GameState status = game->gameInfo->state;
  int x = game->figure->x;
  int y = game->figure->y;

  ck_assert_int_eq(status, Moving);
  ck_assert_int_eq(x, 3);
  ck_assert_int_eq(y, 1);

  freeGame(game);
}
END_TEST

START_TEST(moving_rotate) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->player->action = check_symbol(' ');
  calculate(game);
  GameState status = game->gameInfo->state;
  int x = game->figure->x;
  int y = game->figure->y;

  ck_assert_int_eq(status, Moving);
  ck_assert_int_eq(x, 3);
  ck_assert_int_eq(y, 0);

  freeGame(game);
}
END_TEST

START_TEST(collision_1) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);
  game->gameInfo->ticks_left = 0;
  game->figure->x = 10;
  int result = collision(game);

  ck_assert_int_eq(result, 1);

  freeGame(game);
}
END_TEST

START_TEST(collision_2) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);

  game->gameInfo->ticks_left = 0;
  game->figure->y = FIELD_HEIGHT + 10;
  game->player->action = ACTION;
  calculate(game);

  GameState status = game->gameInfo->state;

  ck_assert_int_eq(status, Spawn);

  freeGame(game);
}
END_TEST

START_TEST(countScore_1) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);

  for (int i = FIELD_HEIGHT - 1; i > FIELD_HEIGHT - 2; --i)
    for (int j = 0; j < FIELD_WIDTH; ++j) game->field->blocks[i][j].block = 1;

  countScore(game);
  int score = game->gameInfo->score;

  ck_assert_int_eq(score, 100);

  freeGame(game);
}
END_TEST

START_TEST(countScore_2) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);

  for (int i = FIELD_HEIGHT - 1; i > FIELD_HEIGHT - 3; --i)
    for (int j = 0; j < FIELD_WIDTH; ++j) game->field->blocks[i][j].block = 1;

  countScore(game);
  int score = game->gameInfo->score;

  ck_assert_int_eq(score, 300);

  freeGame(game);
}
END_TEST

START_TEST(countScore_3) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);

  for (int i = FIELD_HEIGHT - 1; i > FIELD_HEIGHT - 4; --i)
    for (int j = 0; j < FIELD_WIDTH; ++j) game->field->blocks[i][j].block = 1;

  countScore(game);
  int score = game->gameInfo->score;

  ck_assert_int_eq(score, 700);

  freeGame(game);
}
END_TEST

START_TEST(countScore_4) {
  Game *game = initGame();

  game->player->action = check_symbol('\n');
  calculate(game);

  for (int i = FIELD_HEIGHT - 1; i > FIELD_HEIGHT - 5; --i)
    for (int j = 0; j < FIELD_WIDTH; ++j) game->field->blocks[i][j].block = 1;

  countScore(game);
  int score = game->gameInfo->score;

  ck_assert_int_eq(score, 1500);

  freeGame(game);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris_suite");
  TCase *tc = tcase_create("tetris_tc");
  tcase_add_test(tc, init_1);
  tcase_add_test(tc, init_2);
  tcase_add_test(tc, init_3);
  tcase_add_test(tc, status_1);
  tcase_add_test(tc, status_2);
  tcase_add_test(tc, terminate);
  tcase_add_test(tc, moving_left);
  tcase_add_test(tc, moving_right);
  tcase_add_test(tc, moving_down);
  tcase_add_test(tc, moving_action);
  tcase_add_test(tc, moving_rotate);
  tcase_add_test(tc, collision_1);
  tcase_add_test(tc, collision_2);
  tcase_add_test(tc, pause_1);
  tcase_add_test(tc, pause_2);
  tcase_add_test(tc, countScore_1);
  tcase_add_test(tc, countScore_2);
  tcase_add_test(tc, countScore_3);
  tcase_add_test(tc, countScore_4);

  suite_add_tcase(s, tc);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  int tf = 0;

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  tf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return tf > 0;
}

UserAction check_symbol(char ch) {
  UserAction action;
  switch (ch) {
    case ' ':
      action = ROTATE;
      break;
    case 66:
      action = DOWN;
      break;
    case 67:
      action = RIGHT;
      break;
    case 68:
      action = LEFT;
      break;
    case '\n':
      action = START;
      break;
    case 'p':
      action = PAUSE;
      break;
    case 'q':
      action = TERMINATE;
      break;
    default:
      action = ACTION;
      break;
  }
  return action;
}
