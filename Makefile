SOURCE             = src/*
TARGET             = build/tag
INSTALL            = /usr/bin/tag

SUCCESS_MSG        = '  [\e[32m DONE \e[0m]'

CC                 = gcc
CFLAGS             = -std=c99 -Wall -Wextra -g


all: build

build: $(TARGET)

$(TARGET): $(SOURCE)
	@echo 'Building target:'
	@mkdir -p build/
	@$(CC) $(CFLAGS) $^ -o $@
	@echo -e $(SUCCESS_MSG)

clean:
	@echo 'Cleaning workspace:'
	@rm -rf build/
	@echo -e $(SUCCESS_MSG)

rebuild: clean build

install: build
	@echo 'Installing target:'
	@cp -f $(TARGET) $(INSTALL)
	@echo -e $(SUCCESS_MSG)

uninstall:
	@echo 'Uninstalling target:'
	@rm -f $(INSTALL)
	@echo -e $(SUCCESS_MSG)

