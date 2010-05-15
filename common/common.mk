# The make manual: http://www.gnu.org/software/make/manual/make.html

# PROJECT_NAME is the name of the current directory.
PROJECT_NAME = $(notdir $(abspath .))

DEVICE     = atmega2560
CLOCK      = 8000000
PROGRAMMER = -c jtagmkII -P usb

OBJECTS = $(PROJECT_NAME).o

vpath %.c ../common
vpath %.h ../common

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -I../common -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

.PHONY: all
all: $(PROJECT_NAME).hex

# For each FOO in COMMON_MODULES, include ../common/FOO.mk
ifdef COMMON_MODULES
include $(addprefix ../common/,$(COMMON_MODULES:=.mk))
endif

# Note that this implicit rule must come AFTER explicit rules
# like those defined by the common modules.
.c.o:
	$(COMPILE) -c $< -o $@

.PHONY: install
install: all
	$(AVRDUDE) -U flash:w:$(PROJECT_NAME).hex:i

.PHONY: clean
clean:
	rm -f *.hex *.elf *.o

$(PROJECT_NAME).elf: $(OBJECTS)
	$(COMPILE) -o $(PROJECT_NAME).elf $(OBJECTS)

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	rm -f $(PROJECT_NAME).hex
	avr-objcopy -j .text -j .data -O ihex $(PROJECT_NAME).elf $(PROJECT_NAME).hex
