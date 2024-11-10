PROJECT_PATH = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME = $(notdir $(patsubst %/,%, $(PROJECT_PATH)))
BASE_DIR = $(abspath $(PROJECT_PATH)/../..)
ELF_FILE = $(abspath $(PROJECT_PATH)/bin/$(PROJECT_NAME).elf)

.PHONY: build
build:
	@ make -C $(BASE_DIR) build OUTPUT_NAME=$(PROJECT_NAME) PROJECT_PATH=$(PROJECT_PATH)

.PHONY: clean
clean:
	@ rm -r bin 2> /dev/null || true
	@ rm -r obj 2> /dev/null || true

.PHONY: flash
flash: $(ELF_FILE)
	@ wchisp flash $(ELF_FILE)
