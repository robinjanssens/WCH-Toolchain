PROJECT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME = $(notdir $(patsubst %/,%, $(PROJECT_DIR)))
BASE_DIR = $(abspath $(PROJECT_DIR)/../..)
BIN_FOLDER = $(abspath $(PROJECT_DIR)/bin)
OBJ_FOLDER = $(abspath $(PROJECT_DIR)/obj)
ELF_FILE = $(BIN_FOLDER)/$(PROJECT_NAME).elf

.PHONY: build
build:
	@ make -C $(BASE_DIR) build OUTPUT_NAME=$(PROJECT_NAME) BIN_FOLDER=$(BIN_FOLDER) OBJ_FOLDER=$(OBJ_FOLDER)

.PHONY: clean
clean:
	@ rm -r bin 2> /dev/null || true
	@ rm -r obj 2> /dev/null || true

.PHONY: flash
flash: $(ELF_FILE)
	@ wchisp flash $(ELF_FILE)
