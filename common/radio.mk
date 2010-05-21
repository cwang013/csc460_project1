OBJECTS += radio.o \
           spi.o

radio.o: radio.h
spi.o: spi.h
radio.h: nRF24L01.h packet.h spi.h
