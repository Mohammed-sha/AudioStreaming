#include "AudioServer.h"
//Creates a server structure to use. Takes care of overhead
struct asserver* createserverTCP(int port){
	struct asserver* ns = (struct asserver*) malloc(sizeof(struct asserver));
	//Create a socket to be used. Only for initial connection between server and client
	ns->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//setup the data structure
	bzero(&(ns->server), sizeof(struct sockaddr_in));
	ns->server.sin_family = AF_INET;
	ns->server.sin_addr.s_addr = INADDR_ANY;
	ns->server.sin_port = htons(port);
	//bind the socket
	if(bind(ns->sock, (struct sockaddr*) &(ns->server), sizeof(struct sockaddr_in)) <0){
		return NULL;
	}
	//Now start listening for a connection
	listen(ns->sock, 5);
	
	//Setup the message buffer
	ns->msgbuf = (char*) malloc(sizeof(char)*1024);
	return ns;
}


struct asserver* createserverUDP(int port){
	struct asserver* ns = (struct asserver*) malloc(sizeof(struct asserver));
	//Create a socket to be used. Only for initial connection between server and client
	ns->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//setup the data structure
	bzero(&(ns->server), sizeof(struct sockaddr_in));
	ns->server.sin_family = AF_INET;
	ns->server.sin_addr.s_addr = INADDR_ANY;
	ns->server.sin_port = htons(port);
	//bind the socket
	if(bind(ns->sock, (struct sockaddr*) &(ns->server), sizeof(struct sockaddr_in)) <0){
	int err = errno;
	printf("%s\n", strerror(err));
		return NULL;
	}

	
	//Setup the message buffer
	ns->msgbuf = (char*) malloc(sizeof(char)*1024);
	return ns;
}
