CC        = gcc
CFLAGS    = -std=c99 -Wall -Wextra
TARGET    = build/tag
SOURCE    = src/*
INSTALL_DIR   = /usr/bin/
SUCCESS_MSG = "  [\e[32m DONE \e[0m]"

all: build

build: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "Building target:"
	@mkdir -p build/
	@$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo -e $(SUCCESS_MSG)

clean:
	@echo "Cleaning workspace:"
	@rm -rf build/
	@echo -e $(SUCCESS_MSG)

rebuild: clean build

install: build
	@echo "Installing target:"
	cp -f $(TARGET) $(INSTALL_DIR)
	@echo -e $(SUCCESS_MSG)

uninstall:
	@echo "Uninstalling target:"
	@rm -f $(INSTALL_DIR)$(TARGET)
	@echo -e $(SUCCESS_MSG)

archive: build
	@echo "Archiving source:"
	#@rm -f archive.tar.bz2
	#@tar -cjf archive.tar.bz2 $(SOURCE) Makefile README*
	@echo -e $(SUCCESS_MSG)


