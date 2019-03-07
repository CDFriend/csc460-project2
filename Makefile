# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Generic Makefile for AVR C development
# @author Charlie Friend
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OUT=helloworld
BUILD=./build

CC=avr-gcc
OBJCOPY=avr-objcopy

MMCU=atmega2560

SOURCES = \
	main.c \
	tta.c

all: $(BUILD)/$(OUT).hex

upload: $(BUILD)/$(OUT).hex
	MMCU=$(MMCU) ./utils/program.py $(BUILD)/$(OUT).hex

$(BUILD)/$(OUT).hex: $(BUILD)/$(OUT).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BUILD)/$(OUT).elf: $(addprefix $(BUILD)/,$(SOURCES:.c=.o))
	$(CC) -mmcu=$(MMCU) -o $@ $^

$(BUILD)/%.o: %.c $(BUILD)
	$(CC) -mmcu=$(MMCU) -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)