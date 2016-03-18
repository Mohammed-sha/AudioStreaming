#include <math.h>
#include "AudioServer.h"


void transferfile(struct asserver* server){
	printf("Waiting for all clear\n");
	//Wait for the client to give us the all clear
	while(status != CLEAR){	
		//Simply wait
	}
	status = TRANSFERING;
	//Check to make sure it is clear to start
	//Read a payload sized packet until the end of the file
	printf("Transfering file\n");
	ualarm(pkspc*1000, 0);
	while(status == TRANSFERING){
	}
	printf("Finished transfering file\n");
	return;
}
