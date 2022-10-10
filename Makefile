Q ?= @
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
RANLIB = arm-none-eabi-ranlib
NWLINK = nwlink
LINK_GC = 1
LTO = 1

LIBS_PATH=$(shell pwd)/output/libs

CFLAGS += $(shell $(NWLINK) eadk-cflags)
CFLAGS += -Os
CPPFLAGS += -I$(LIBS_PATH)/include
CFLAGS += -fno-exceptions -fno-unwind-tables

LDFLAGS += --specs=nano.specs
LDFLAGS += -L$(LIBS_PATH)/lib

ifeq ($(LINK_GC),1)
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,-e,main -Wl,-u,eadk_app_name -Wl,-u,eadk_app_icon -Wl,-u,eadk_api_level
LDFLAGS += -Wl,--gc-sections
endif

ifeq ($(LTO),1)
AR = arm-none-eabi-gcc-ar
RANLIB = arm-none-eabi-gcc-ranlib
CFLAGS += -flto -fno-fat-lto-objects
CFLAGS += -fwhole-program
CFLAGS += -fvisibility=internal
LDFLAGS += -flinker-output=nolto-rel
endif

.PHONY: build
build: output/peanutgb.nwa

.PHONY: check
check: output/peanutgb.bin

.PHONY: run
run: output/peanutgb.nwa src/flappyboy.gb
	@echo "INSTALL $<"
	$(Q) $(NWLINK) install-nwa --external-data src/flappyboy.gb $<

output/%.bin: output/%.nwa src/flappyboy.gb
	@echo "BIN     $@"
	$(Q) $(NWLINK) nwa-bin --external-data src/flappyboy.gb $< $@

output/%.elf: output/%.nwa src/flappyboy.gb
	@echo "ELF     $@"
	$(Q) $(NWLINK) nwa-elf --external-data src/flappyboy.gb $< $@

output/peanutgb.nwa: output/main.o output/icon.o
	@echo "LD      $@"
	$(Q) $(CC) $(CPPFLAGS) $(CFLAGS) -Wl,--relocatable -nostartfiles $(LDFLAGS) $^ -o $@

$(addprefix output/,%.o): src/%.c
	@mkdir -p $(@D)
	@echo "CC      $@"
	$(Q) $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

output/icon.o: src/icon.png
	@echo "ICON    $<"
	$(Q) $(NWLINK) png-icon-o $< $@

.PHONY: clean
clean:
	@echo "CLEAN"
	$(Q) rm -rf output
