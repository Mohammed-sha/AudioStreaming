#include "AudioClient.h"

struct datamessage* UDPrecv(struct asclient* cl, int pyldsize){
	int len = sizeof(struct sockaddr_in);
	struct datamessage* nm = (struct datamessage*) malloc(sizeof(struct datamessage)); //New message to return
	//Get enough memory from the heap for the expected max payload size
	nm->msg = (char*) malloc(sizeof(char)*pyldsize);
	//Now receive a message from the UDP connection
	nm->size = recvfrom(cl->sock, nm->msg, pyldsize, 0,(struct sockaddr*)  &(cl->server), &len);
	if(nm->size > 0){
		return nm;
	}else{
		return NULL;
	}
}
