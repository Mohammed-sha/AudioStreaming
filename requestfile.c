#include "AudioClient.h"

int requestfile(struct asclient* cl, char* file){
	int db;

	//send a message to the server for the requested file
	db = write(cl->sock, (char*) file, sizeof(char)*strlen(file));
	//get the message back 
	db = read(cl->sock, cl->msgbuf, sizeof(char)*1024);
	//now check to see if it was a valid request
	if(strncmp(cl->msgbuf, "OK", 2) != 0){
		return -1;
	}
	//We had a valid request pull out the port
	char* port = (char*) malloc(sizeof(char)*6);
	bzero(port, 6);
	memcpy(port,&(cl->msgbuf[2]), 5);
	return atoi(port);
}
