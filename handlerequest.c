#include "AudioServer.h"

//Get a connection and check to see if the requested file is valid
int handlerequest(struct asserver* server, char* udpport){
	int len = sizeof(struct sockaddr_in);
	//Accept a connection
	int tmpsock;
	printf("Waiting for request\n");
	tmpsock = accept(server->sock, (struct sockaddr*) &(server->client), &len);
	if(tmpsock < 0){
		int err = errno;
		printf("Error while accepting connection %s\n", strerror(err));
		return -1;
	}
	//flush the message buffer
	bzero(server->msgbuf, 1024);
	//read a message from the socket
	read(tmpsock,server->msgbuf, 1024);
	//print the requested file path
	printf("Requested file: %s\n", server->msgbuf);
	if(access(server->msgbuf, R_OK) < 0){
		write(tmpsock, "INVALID", strlen("INVALID"));
		printf("Invalid request\n");
		close(tmpsock);
		return -1;
	}else{
		printf("File is valid\n");
		char* sndport = (char*) malloc(sizeof(char)*9);
		bzero(sndport, 9);
		strcat(sndport, "OK");
		strcat(sndport,udpport);
		write(tmpsock, sndport, 9);
		close(tmpsock);
		return 1;
	}
}

