#include "AudioServer.h"

void UDPsend(struct asserver* server, struct datamessage* msg){
	//Send the message to the server
	sendto(server->sock, msg->msg, msg->size,0,(struct sockaddr*) &(server->client), sizeof(struct sockaddr_in));
}
