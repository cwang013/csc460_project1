# TODO: base this makefile on http://www.slac.stanford.edu/comp/unix/gnu-info/make_2.html#SEC9
# and my ooc makefile.

# TODO: have a "common" directory for source that's common to all OSes, and one directory
# for each OS. Ech OS directory should have it's own makefile. These makefiles could include
# several common .mk files. All the general rules and variables should go in common/common.mk.

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
OBJECTS    = $(PROJECT_NAME).o


# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all: $(PROJECT_NAME).hex

.c.o:
	$(COMPILE) -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(PROJECT_NAME).hex:i

clean:
	rm -f $(PROJECT_NAME).hex $(PROJECT_NAME).elf $(OBJECTS)

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
