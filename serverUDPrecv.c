#include "AudioServer.h"

struct datamessage* UDPrecv(struct asserver* server, int pyldsize){
	int len = sizeof(struct sockaddr_in);
	//Create a new datamessage on the heap
	struct datamessage* nm = (struct datamessage*) malloc(sizeof(struct datamessage));
	//Allocate enough space for the message
	nm->msg = (char*) malloc(sizeof(char)*pyldsize);
	//Receive message from connection
	nm->size = recvfrom(server->sock, nm->msg, pyldsize, 0, (struct sockaddr*) &(server->client), &len);
	if(nm->size > 0){
		return nm;
	}else{
		return NULL;
	}
}
