#include "AudioClient.h"

struct asclient* createclientTCP(char* address, int port){
	printf("Passed address and port: %s %d %d\n", address, port, htons(port));
	struct asclient* nc = (struct asclient*) malloc(sizeof(struct asclient)); //new client
	nc->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nc->sock < 0){
		return NULL;
	}
	//load the server in
	bzero(&(nc->server), sizeof(struct sockaddr_in));
	nc->server.sin_family = AF_INET;
	inet_aton((char*) address, &(nc->server.sin_addr));
	nc->server.sin_port = htons(port);
	nc->port = port;
	//Create the buffer
	nc->msgbuf = (char*) malloc(sizeof(char)*1024);
	memset(nc->msgbuf, '\0', 1024);
	//Connect to the server
	if(connect(nc->sock, (struct sockaddr*) &(nc->server), sizeof(struct sockaddr_in)) < 0){
		free(nc);
		return NULL;
	}
	return nc;
}


struct asclient* createclientUDP(char* address, int port){
	struct asclient* nc = (struct asclient*) malloc(sizeof(struct asclient)); //new client
	nc->sock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
	//load the server in
	bzero(&(nc->server), sizeof(struct sockaddr_in));
	nc->server.sin_family = AF_INET;
	inet_aton((char*) address, &(nc->server.sin_addr));
	nc->server.sin_port = htons(port);
	nc->port = port;
	//Create the buffer
	nc->msgbuf = (char*) malloc(sizeof(char)*1024);
	memset(nc->msgbuf, '\0', 1024);
	return nc;
}
