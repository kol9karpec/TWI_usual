DEVICE 	= atmega2560
F_CPU 	= 16000000
PORT = /dev/ttyACM0
SPEED = 115200

SRC =	main.c \
		./src/usart.c \
		./src/twi_usual.c \
		./src/bh1750.c

all:
	avr-gcc -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(DEVICE) $(SRC)
	avr-objcopy -j .text -j .data -O ihex a.out main.hex
	avr-size --format=avr --mcu=atmega2560 main.hex

flash:
	avrdude -v -p$(DEVICE) -cwiring -P$(PORT) -b$(SPEED) -D -V -U flash:w:main.hex:i

clean:
	@rm -f *.o
	@rm -f *.elf
	@rm -f *~
	@rm -f *.out
	@rm -f *.hex
	@echo clean successfully
