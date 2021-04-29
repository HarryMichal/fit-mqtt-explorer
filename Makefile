# Makefile
# Ondřej Míchal <xmicha80>

NAME = fit-mqtt-client
ZIPFILE = 1-xmicha80-xgross11.zip

BUILD_DIR = ./build
SRC_DIR = ./src
DOC_DIR = ./doc
DATA_DIR = ./data

TARGET := $(BUILD_DIR)/$(NAME)

.PHONY: all run doc doxygen zip clean help

all:
	cmake -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

run: all
	@$(TARGET)

doc:
	doxygen ./Doxyfile

zip:
	zip -r $(ZIPFILE) README.md README.txt Doxyfile Makefile CMakeLists.txt resources.qrc $(SRC_DIR) $(DATA_DIR)

clean:
	@rm -rf $(ZIPFILE) $(BUILD_DIR) $(DOC_DIR)

help:
	@echo "make"
	@echo "    Execute 'make all'"
	@echo "make all"
	@echo "    Build the project"
	@echo "make run"
	@echo "    Run the program"
	@echo "make doc"
	@echo "    Generate documentation using doxygen"
	@echo "make doxygen"
	@echo "    Alias to 'make doc'"
	@echo "make zip"
	@echo "    Create a zip file with the project"
	@echo "make clean"
	@echo "    Remove all unneeded files"
	@echo "make help"
	@echo "    Show this screen"
