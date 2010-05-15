# The make manual: http://www.gnu.org/software/make/manual/make.html

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.

PROJECT_NAME = $(notdir $(abspath .))

DEVICE     = atmega2560
CLOCK      = 8000000
PROGRAMMER = -c jtagmkII -P usb

OBJECTS += $(PROJECT_NAME).o

vpath %.c ../common
vpath %.h ../common

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -I../common -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all: $(PROJECT_NAME).hex

ifdef COMMON_MODULES
include $(addprefix ../common/,$(COMMON_MODULES:=.mk))
endif

.c.o:
	$(COMPILE) -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(PROJECT_NAME).hex:i

clean:
	rm -f *.hex *.elf *.o

# file targets:
$(PROJECT_NAME).elf: $(OBJECTS)
	$(COMPILE) -o $(PROJECT_NAME).elf $(OBJECTS)

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	rm -f $(PROJECT_NAME).hex
	avr-objcopy -j .text -j .data -O ihex $(PROJECT_NAME).elf $(PROJECT_NAME).hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm: $(PROJECT_NAME).elf
	avr-objdump -d $(PROJECT_NAME).elf
