#include "AudioClient.h"


void UDPsend(struct asclient* cl, struct datamessage* msg){
	sendto(cl->sock, msg->msg, msg->size,0,(struct sockaddr*) &(cl->server), sizeof(struct sockaddr_in));
}
