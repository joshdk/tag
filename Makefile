OUTFILES           = build/main.o build/dsv.o build/tag.o build/util.o
TARGET             = build/tag
INSTALL            = /usr/bin/tag

SUCCESS_MSG        = '  [\e[32m DONE \e[0m]'

CC                 = gcc
CFLAGS             = -std=c99 -Wall -Wextra -g


all: build

build: $(TARGET)

$(TARGET): $(OUTFILES) src/debug.h src/res.h
	@echo 'Building target:'
	@$(CC) $(CFLAGS) $^ -o $@
	@echo -e $(SUCCESS_MSG)

build/%.o: src/%.c
	@mkdir -p build/
	@$(CC) $(CFLAGS) -c $< -o $@

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

