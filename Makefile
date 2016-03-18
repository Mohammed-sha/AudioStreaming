CC = gcc

CFLAGS = -c -g 

LFLAGS = -o 


CFILES = $(wildcard *.c)

all: compile myaudiostream myaudiostreamd

compile: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES)

myaudiostream: 
	$(CC) $(LFLAGS) myaudiostream myaudiostream.o createclient.o clientUDPrecv.o requestfile.o streamaudiofile.o clientUDPsend.o receivedata.o playback.o -lpthread -lrt


myaudiostreamd:
	$(CC) $(LFLAGS) myaudiostreamd myaudiostreamd.o createserver.o serverUDPsend.o handlerequest.o transferfile.o serverUDPrecv.o servercontrol.o sendpacket.o -lm


clean:
	@rm *.o
	@rm myaudiostreamd
	@rm myaudiostream
	@rm client.log
	@rm server.log
