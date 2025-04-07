# Переменные
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fsanitize=address -g
LDFLAGS = 
INCLUDES = -Iinclude
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/statdata_tool
TEST_TARGET = $(BIN_DIR)/statdata_test

# Каталоги исходных файлов
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test

# Исходники
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/oi.c $(SRC_DIR)/process.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Тестовые исходники
TEST_SRCS = $(TEST_DIR)/tests.c $(SRC_DIR)/oi.c $(SRC_DIR)/process.c
TEST_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Статический анализ с cppcheck
CPPCHECK = cppcheck
CPPCHECK_FLAGS = --enable=all --inconclusive --force --std=c99 --quiet

# Создание директории bin, если ее нет
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Создание директории build, если ее нет
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Правило для компиляции .c в .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Основная цель - сборка программы
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Цель для тестирования
test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST_OBJS) -o $@ $(LDFLAGS)

# Цель для статического анализа с cppcheck
cppcheck:
	$(CPPCHECK) $(SRC_DIR) $(TEST_DIR) $(INCLUDE_DIR)

# Очистка всех скомпилированных файлов
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Установки по умолчанию для всех целей
.PHONY: all clean test cppcheck
