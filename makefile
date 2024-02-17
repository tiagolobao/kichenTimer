# Make file based on 
# https://github.com/peakhunt/freertos_atmega328p/blob/master/Makefile
# and modified by Tiago Lobao
#
# 
# make (default) = compile project
#
# make clean = delete all binaries
#
# make program = flash through AVRDUDE
#

# ---------------------------------------
# -------- MAKEFILE USER DEFINES --------
# ---------------------------------------

# ---------------------------------------
# Microcontroller specific
MCU = atmega328p
F_CPU = 16000000UL

# ---------------------------------------
# Directiory for the freeRTOS
FREERTOS_DIR = ./miniAVRfreeRTOS

# ---------------------------------------
# Flashing options
# using standard arduino bootloader
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_BAUDRATE = 115200

# ---------------------------------------
# Target file name options
OUTPUT_FOLDER = out
TARGET = main

# ---------------------------------------
# compiler / programmer options
CC = avr-gcc
AVRDUDE = avrdude

# ---------------------------------------
# Sources/includes to be used
FREERTOS_SRC :=						\
	$(FREERTOS_DIR)/list.c			\
	$(FREERTOS_DIR)/timers.c		\
	$(FREERTOS_DIR)/stream_buffer.c	\
	$(FREERTOS_DIR)/heap_3.c		\
	$(FREERTOS_DIR)/event_groups.c	\
	$(FREERTOS_DIR)/hooks.c			\
	$(FREERTOS_DIR)/port.c			\
	$(FREERTOS_DIR)/queue.c			\
	$(FREERTOS_DIR)/tasks.c

APPLICATION_SRC :=	./src/main.c \
					./src/drivers/uart.c \
					./src/tasks/debuglog.c

SOURCES :=	$(FREERTOS_SRC) \
			$(APPLICATION_SRC)

INC_PATH := -I$(FREERTOS_DIR) -I./src -I./src/drivers -I./src/tasks
 
OBJECT_FILES := $(SOURCES:%.c=%.o)

# ---------------------------------------
# GCC and AVRDUDE arguments
AVRDUDE_WRITE_FLASH := -U flash:w:./$(OUTPUT_FOLDER)/$(TARGET).hex

AVRDUDE_FLAGS :=	-p $(MCU) -b $(AVRDUDE_BAUDRATE)	\
					-P $(AVRDUDE_PORT)					\
					-c $(AVRDUDE_PROGRAMMER)

MCUFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU)

CFLAGS := $(MCUFLAGS) $(INC_PATH) -ffunction-sections -O2
LFLAGS := $(MCUFLAGS) -Wl,--gc-sections

# ---------------------------------------
# ---------- TARGETS --------------------
# ---------------------------------------
.PHONY: all .program .clean

all: .clean $(TARGET)

.init:
	@echo Starting build...
	@echo $(SOURCES)
	@echo $(OBJECT_FILES)
	@mkdir $(OUTPUT_FOLDER)


$(OBJECT_FILES): %.o: %.c
	@echo building $@...
	$(CC) $(CFLAGS) -c -o $@ $<
	mv $@ $(OUTPUT_FOLDER)/


TARGET_PATH := ./$(OUTPUT_FOLDER)/$(TARGET)

$(TARGET): .init $(OBJECT_FILES)
	@echo Linking...
	avr-nm -n ./$(OUTPUT_FOLDER)/*.o > $(TARGET_PATH).sym
	$(CC) $(LFLAGS) ./$(OUTPUT_FOLDER)/*.o -o $(TARGET_PATH).elf
	avr-objcopy -O ihex $(TARGET_PATH).elf $(TARGET_PATH).hex
	avr-objcopy -O ihex $(TARGET_PATH).hex -R .eeprom $(TARGET_PATH)_eeprom.hex
	avr-objcopy -I ihex $(TARGET_PATH).hex -O binary $(TARGET_PATH).bin
	avr-size --format=berkeley $(TARGET_PATH).hex

.program:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

.clean:
	@echo "Cleaning output"
	rm -rf ./$(OUTPUT_FOLDER)
