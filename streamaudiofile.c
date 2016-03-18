#include "AudioClient.h"

//Note to self:
//Until the stream is working simply read the data from the UDP socket and print it to a file.
//use diff afterwards to check they are the same. Once it all works simply change things to 
//write to /dev/audio instead
void streamaudiofile(struct asclient* cl, int pyldsize){
	//Create a file pointer
	FILE* tmp = fopen("audio.tmp", "w");
	//Until the we get the signal to stop write the data to the file
	struct datamessage *msg = (struct datamessage*) malloc(sizeof(struct datamessage));
	msg->msg = (char*) malloc(sizeof(char)*6); //Malloc enough memory for CLEAR message
	//Let the server know we are ready to start
	msg->msg = "CLEAR";
	msg->size = 6;
	UDPsend(cl, msg); //Send the all clear
	free(msg); 
	msg = NULL;
	//While the message does not equal END write the data to the file
	while((msg = UDPrecv(cl, pyldsize)) != NULL && strncmp(msg->msg, "END", 3) != 0){
		printf("Received %d bytes\n", msg->size);
		fwrite(msg->msg, sizeof(char), msg->size, tmp);
	}
	fclose(tmp);	
}
