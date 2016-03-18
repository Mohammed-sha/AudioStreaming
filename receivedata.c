#include <math.h>
#include "AudioClient.h"


//This function takes as it's arguments a string to be stored it's lenght
//and two ints that will be stored in a single character array
char* storeinfo(char* st,int stlen ,int num1, int num2){
	char* ar = (char*) malloc(sizeof(char)*(stlen+8));
	memcpy(ar, st, stlen);
	memcpy(ar+stlen, &num1, 4);
	memcpy(ar+stlen+4, &num2, 4);
	return ar;
}



void receivedata(int sig, siginfo_t* i, void* unused){
	//Read from the socket
	struct datamessage* msg = UDPrecv(cl, pyldsize);
	if(strncmp(msg->msg, "END", 3) == 0){
		//set stream to 0 and allow the program to exit
		printf("Received End message\n");
		stream = 0;
		return;
	}
	//We need to handle the cases of buffer being too full or too empty
	if(bflvl > targbuf){
		//We are too full let the server know
		char* bckoff = "ADJUST-";//Server expects a string letting it know it needs to adjust 
		int diff = targbuf-bflvl; //This is Q* - Q(t)
		int tau = pbrate; //This is our current 1/gamma
		struct datamessage msg; //Our message to the server
		msg.msg = storeinfo(bckoff,7, diff, tau); //This puts the message and numbers into a character buffer
		msg.size = 7+4+4; //The size of the message is the length of ADJUST- and 2 32-bit integers
		UDPsend(cl, &msg); //Send the message
		free(msg.msg); //Free the memory	
	}else if(bflvl < targbuf){
		//We are too full let the server know
		char* bckoff = "ADJUST+";//Server expects a string letting it know it needs to adjust 
		int diff = targbuf-bflvl; //This is Q* - Q(t)
		int tau = pbrate; //This is our current 1/gamma
		struct datamessage msg; //Our message to the server
		msg.msg = storeinfo(bckoff,7, diff, tau); //This puts the message and numbers into a character buffer
		msg.size = 7+4+4; //The size of the message is the length of ADJUST- and 2 32-bit integers
		UDPsend(cl, &msg); //Send the message
		free(msg.msg); //Free the memory	
	}
	//Don't copy anything until we let the server know our current state
	memcpy(&(databuffer[bflvl]), msg->msg, msg->size);
	bflvl +=msg->size;
	sem_post(&lock);//This should be the last thing done. We do not want changes happening to the buffer while we are using it
	struct timeval tz; //Timezone for log file
	gettimeofday(&tz, NULL);
	fprintf(audiolog, "%d.%d %d\n", (int)tz.tv_sec - (int)start.tv_sec,(int)tz.tv_usec, bflvl);
}

