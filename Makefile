SOURCE             = src/*
TARGET             = build/tag
INSTALL_DIR        = /usr/bin

SUCCESS_MSG        = '  [\e[32m DONE \e[0m]'

CC                 = gcc
CFLAGS             = -std=c99 -Wall -Wextra -g


all: build

build: $(TARGET) 

$(TARGET): $(SOURCE)
	@echo 'Building target:'
	@mkdir -p build/
	@$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo -e $(SUCCESS_MSG)

clean:
	@echo 'Cleaning workspace:'
	@rm -rf $(TARGET)
	@echo -e $(SUCCESS_MSG)

rebuild: clean build

install: build
	@echo 'Installing target:'
	@cp -f $(TARGET) $(INSTALL_DIR)
	@echo -e $(SUCCESS_MSG)

uninstall:
	@echo 'Uninstalling target:'
	@rm -f $(INSTALL_DIR)/$(TARGET)
	@echo -e $(SUCCESS_MSG)

