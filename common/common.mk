# The make manual: http://www.gnu.org/software/make/manual/make.html

# PROJECT_NAME is the name of the current directory.
PROJECT_NAME = $(notdir $(abspath .))

DEVICE     = atmega2560
CLOCK      = 8000000
PROGRAMMER = -c jtagmkII -P usb

OBJECTS = $(PROJECT_NAME).o

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -I../common -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

.PHONY: all
all: $(PROJECT_NAME).hex

ifdef COMMON_MODULES
# Add common directory to search path for *.[ch] files.
vpath %.c ../common
vpath %.h ../common
# Include ../common/FOO.mk for each FOO in COMMON_MODULES.
include $(addprefix ../common/,$(COMMON_MODULES:=.mk))
endif

# Note that this pattern rule must come AFTER static rules
# like those defined by the common modules (e.g. uart.o).
%.o: %.c
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
