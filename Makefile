## General Flags
PROJECT = gol
PRGDEV = /dev/ttyUSB2
MCU = atmega328p
F_CPU = 8000000UL
TARGET = $(PROJECT).elf
OBJCPY = avr-objcopy 
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU) 

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) 
CFLAGS += -Wall -std=gnu99  -DF_CPU=$(F_CPU) -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I. 

## Objects that must be built in order to link
OBJECTS = $(PROJECT).o uart.o bootloader.o main.o lcd_dogm128_spi.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex size

## Compile
$(PROJECT).o: $(PROJECT).c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

## Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	$(OBJCPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@
	$(OBJCPY) -I ihex -O binary $(PROJECT).hex $(PROJECT).bin


size: $(TARGET)
#	avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) *.elf *.hex *.eep *.lss *.map *.bin *~ jj.jj 



load:
	avrdude -s -F -p $(MCU) -b 38400 -P $(PRGDEV) -c stk500v2 -e -U flash:w:$(PROJECT).hex
	