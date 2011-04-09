SOURCE             = tag.1
TARGET             = tag.1.gz
INSTALL            = /usr/share/man/man1/tag.1.gz

SUCCESS_MSG        = '  [\e[32m DONE \e[0m]'


all: build

build: $(TARGET)

$(TARGET): $(SOURCE)
	@echo 'Building tag manpage:'
	@gzip -c $(SOURCE) > $(TARGET)
	@echo -e $(SUCCESS_MSG)

clean:
	@echo 'Cleaning workspace:'
	@rm -f $(TARGET)
	@echo -e $(SUCCESS_MSG)

rebuild: clean build

install: build
	@echo 'Installing tag manpage:'
	@cp -f $(TARGET) $(INSTALL)
	@echo -e $(SUCCESS_MSG)

uninstall:
	@echo 'Uninstalling tag manpage:'
	@rm -f $(INSTALL)
	@echo -e $(SUCCESS_MSG)


