TOOLPREFIX = /opt/homebrew/opt/riscv-gnu-toolchain/bin/riscv64-unknown-elf-

BIN_FOLDER = $(abspath $(PROJECT_PATH)/bin)
OBJ_FOLDER = $(abspath $(PROJECT_PATH)/obj)

ELF_FILE = $(BIN_FOLDER)/$(OUTPUT_NAME).elf
BIN_FILE = $(BIN_FOLDER)/$(OUTPUT_NAME).bin
HEX_FILE = $(BIN_FOLDER)/$(OUTPUT_NAME).hex
SIZE_FILE = $(BIN_FOLDER)/$(OUTPUT_NAME).size
MAP_FILE = $(BIN_FOLDER)/$(OUTPUT_NAME).map
LINKERSCRIPT = devices/lana_tny/linkerscript/ch32v203g6u6.ld

# --- Include S Files ---
S_FILES = devices/lana_tny/startup/startup_ch32v20x_D6.S
S_OFILES = $(foreach file,$(S_FILES),$(OBJ_FOLDER)/$(notdir $(file:.S=.S.o)))  # adapt path/file.S to OBJ_FOLDER/file.S.o

# --- Include C Files ---
C_FILES = \
	devices/lana_tny/src/debug.c \
	devices/lana_tny/src/ch32v20x_gpio.c \
	devices/lana_tny/src/ch32v20x_misc.c \
	devices/lana_tny/src/ch32v20x_rcc.c \
	devices/lana_tny/src/ch32v20x_usart.c \
	devices/lana_tny/src/ch32v20x_it.c \
	devices/lana_tny/src/system_ch32v20x.c \
	devices/lana_tny/src/lana.c \
	$(PROJECT_PATH)/src/main.c
C_OFILES = $(foreach file,$(C_FILES),$(OBJ_FOLDER)/$(notdir $(file:.c=.c.o)))  # adapt path/file.c to OBJ_FOLDER/file.c.o

# --- Include C++ Files ---
CPP_FILES = 
CPP_OFILES = $(foreach file,$(CPP_FILES),$(OBJ_FOLDER)/$(notdir $(file:.cpp=.cpp.o)))  # adapt path/file.cpp to OBJ_FOLDER/file.cpp.o

OFILES = $(C_OFILES) $(CPP_OFILES) $(S_OFILES)

# ------------------------------
# Compiler Flags
# ------------------------------
TARGET_COMMONFLAGS = -march=rv32imac -march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fsigned-char -fno-common -Wunused -Wuninitialized
TARGET_CFLAGS = -std=gnu99
TARGET_CPPFLAGS = -std=gnu99
TARGET_SFLAGS = -x assembler-with-cpp
TARGET_LDFLAGS = --specs=nano.specs --specs=nosys.specs -nostartfiles -T $(LINKERSCRIPT) -L $(dir $(LINKERSCRIPT))
TOOLCHAIN_LDLIBS = -lm
COMMONFLAGS = $(TARGET_COMMONFLAGS) -Og -g3 -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-comment -Wno-unused-function
CFLAGS = $(COMMONFLAGS) $(DEPFLAGS) $(TARGET_CFLAGS) $(INCLUDE_DIRS) -Werror-implicit-function-declaration
CPPFLAGS = $(COMMONFLAGS) $(DEPFLAGS) $(TARGET_CPPFLAGS) $(INCLUDE_DIRS)
SFLAGS = $(COMMONFLAGS) $(DEPFLAGS) $(TARGET_SFLAGS) $(INCLUDE_DIRS)
LDFLAGS = $(COMMONFLAGS) $(TARGET_LDFLAGS) -Wl,-Map=$(MAP_FILE) -Wl,--gc-sections

DEPFLAGS = -MMD -MF $(@:.o=.d) -MP -MQ $@
include $(wildcard $(OFILES:.o=.d))

INCLUDE_DIRS = \
	-Idevices/lana_tny/inc \
	-I$(PROJECT_PATH)/src/ \

# ------------------------------
# Compilation Rules
# ------------------------------
default: build

define compilation-rules
$(1)%.c.o: $(2)%.c
	$$(c-rule)

$(1)%.cpp.o: $(2)%.cpp
	$$(cpp-rule)

$(1)%.s.o: $(2)%.s
	$$(asm-rule)

$(1)%.S.o: $(2)%.S
	$$(asm-rule)

$(1)%.asm.o: $(2)%
	$$(asm-rule)

endef

define c-rule
@ mkdir -p $(OBJ_FOLDER)
$(info Compiling: $<)
@ $(TOOLPREFIX)gcc $(CFLAGS) -c '$<' -o '$@'
endef

define cpp-rule
@ mkdir -p $(OBJ_FOLDER)
$(info Compiling: $<)
@ $(TOOLPREFIX)g++ $(CPPFLAGS) -c '$<' -o '$@'
endef

define asm-rule
@ mkdir -p $(OBJ_FOLDER)
$(info Compiling: $<)
@ $(TOOLPREFIX)gcc $(SFLAGS) -c '$<' -o '$@'
endef

$(eval $(call compilation-rules, $(OBJ_FOLDER), devices/lana_tny/startup/))
$(eval $(call compilation-rules, $(OBJ_FOLDER), devices/lana_tny/src/))
$(eval $(call compilation-rules, $(OBJ_FOLDER), $(PROJECT_PATH)/src/))

# Cancel implicit rules
.SUFFIXES:

# ------------------------------
# Output Targets
# ------------------------------
LD = $(if $(CPP_OFILES), $(TOOLPREFIX)g++, $(TOOLPREFIX)gcc)
$(ELF_FILE): $(OFILES)
	@ mkdir -p $(BIN_FOLDER)
	$(info Linking: $@)
	@ $(LD) $(LDFLAGS) -o $@ $(filter %.o %.a %.so,$^) $(TOOLCHAIN_LDLIBS)

$(BIN_FILE): $(ELF_FILE)
	$(info Creating: $@)
	@ $(TOOLPREFIX)objcopy -O binary $< $@

$(HEX_FILE): $(ELF_FILE)
	$(info Creating: $@)
	@ $(TOOLPREFIX)objcopy -O ihex $< $@

$(SIZE_FILE): $(ELF_FILE)
	$(info Creating: $@)
	@ $(TOOLPREFIX)size $<

# ------------------------------
# Phony Targets
# ------------------------------
.PHONY: build
build: $(ELF_FILE) $(BIN_FILE) $(HEX_FILE) $(SIZE_FILE)
