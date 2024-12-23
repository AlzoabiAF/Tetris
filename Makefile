SHELL = /bin/bash
FLAGS = -Wall -Werror -Wextra -std=c11 -lncurses
FLAG_COV = --coverage 
OS = $(shell uname -s)

BUILD_DIR = build
BACK_DIR = brick_game/tetris
FRONT_DIR = gui/cli
TEST_DIR = tests

MAIN=$(BACK_DIR)/tetris.c
BACK_SRC = $(filter-out $(MAIN), $(wildcard $(BACK_DIR)/*.c))
FRONT_SRC = $(wildcard $(FRONT_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

MAIN_OBJ = $(addprefix $(BUILD_DIR)/, $(MAIN:.c=.o))
BACK_OBJ = $(addprefix $(BUILD_DIR)/, $(BACK_SRC:.c=.o))
FRONT_OBJ = $(addprefix $(BUILD_DIR)/, $(FRONT_SRC:.c=.o))
TEST_OBJ = $(addprefix $(BUILD_DIR)/, $(TEST_SRC:.c=.o))




ifeq ($(OS), Linux)
	TEST_FLAGS = -lcheck -pthread -lrt -lm -lsubunit
	OPEN = xdg-open
else
	TEST_FLAGS = -lcheck
	OPEN = open
endif



all: clean install dvi gcov_report

tetris: $(BACK_OBJ) $(FRONT_OBJ) $(MAIN_OBJ)
	@$(CC) $^ -lncurses -o $(BUILD_DIR)/$@

install: clean tetris
	@echo 0 > high_score.dat

uninstall:
	@rm $(BUILD_DIR)/tetris

dvi: 
	@mkdir -p $(BUILD_DIR)
	@doxygen
	@$(OPEN) $(BUILD_DIR)/doxygen/html/index.html

dist: 
	@cd ../ && mkdir -p dist
	@cp -r $(BACK) ../dist
	@cp -r $(FRONT) ../dist
	@cp -r dvi ../dist/  
	@tar -czvf ../tetris.tar.gz -C ../dist .
	@rm -rf ../dist tetris
	@echo "Distribution package created: /tetris.tar.gz"

test: $(TEST_OBJ)
	@$(CC) $(FLAGS) $(FLAG_COV) -o $(BUILD_DIR)/test $(TEST_OBJ) $(BACK_SRC) ${TEST_FLAGS}
	./$(BUILD_DIR)/test 

gcov_report: test
	lcov -t "test" -o $(BUILD_DIR)/test.info -c -d $(BUILD_DIR)
	genhtml -o $(BUILD_DIR)/report $(BUILD_DIR)/test.info
	$(OPEN) $(BUILD_DIR)/report/index.html


$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	gcc $(FLAGS) -c $<  -o $@;

valgrind_test:
	valgrind --tool=memcheck --leak-check=full ./$(BUILD_DIR)/test 

valgrind_tetris:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(BUILD_DIR)/tetris

clean:
	rm -rf build 

check_format:
	find . -name '*.c' -or -name '*.h' | xargs clang-format -n -style=Google

rebuild: clean all

.PHONY: dvi