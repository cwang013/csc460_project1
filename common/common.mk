# The make manual: http://www.gnu.org/software/make/manual/make.html
# A free book on make: http://oreilly.com/catalog/make3/book/

# PROJECT_NAME is the name of the current directory.
PROJECT_NAME := $(notdir $(CURDIR))

OBJECTS = $(PROJECT_NAME).o

DEVICE     = atmega2560
CLOCK      = 8000000 # 8 MHz
PROGRAMMER = -c jtagmkII -P usb

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)

# These variables are used by make's built-in rules.
CC = avr-gcc
CFLAGS = -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields \
		 -fpack-struct -fshort-enums -DF_CPU=$(CLOCK)
TARGET_ARCH = -mmcu=$(DEVICE)
LDFLAGS = -Wall
SHELL = /bin/bash

.PHONY: all
all: $(PROJECT_NAME).hex

ifdef COMMON_MODULES
# Add the common directory to various search paths.
vpath %.c ../common
vpath %.h ../common
CFLAGS += -I../common
# Include ../common/FOO.mk for each FOO in COMMON_MODULES.
include $(addprefix ../common/,$(COMMON_MODULES:=.mk))
endif

.PHONY: install
install: all
	$(AVRDUDE) -U flash:w:$(PROJECT_NAME).hex:i

.PHONY: clean
clean:
	$(RM) *.hex *.elf *.o

$(PROJECT_NAME).elf: $(OBJECTS)
	$(LINK.o) $^ -o $@

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	$(RM) $@
	avr-objcopy -j .text -j .data -O ihex $< $@

