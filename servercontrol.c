#include <math.h>
#include "AudioServer.h"


void servercontrol(int sig, siginfo_t *i, void* unused){
	//we received a message from the client read the message
	struct datamessage* msg = UDPrecv(tserv, pyldsize);
	float delta1 = 1.1; //Our delta for mode 1. Since we are dealing with lambda we need to make it larger not smaller
	float delta3 = 2; //Our delta for mode 3
	float epsilon2 = .01; //Our epsilon for mode 2
	float epsilon3 = .01; //Our epsilon for mode 3
	//The adjust message is of the following format
	//First the string "ADJUST" followed by a "+" or "-"
	//for if we are increasing or decreasing.
	//Next the message contains the difference
	//between Q* and Q(t) and then gamma(t).
	//This are assumed to be 32 bit integers.
		
	if(strncmp(msg->msg, "CLEAR", strlen("CLEAR"))==0){	
		//We recieved an all clear message from the client
		//Set the appropiate flag
		status = CLEAR;
	}else if(strncmp(msg->msg, "ADJUST-", 7) == 0){
		//We have message telling us we need to less often
		
		//Pull out the data that the client sent to us
		int diff;
		int gamma; //They should transfer the fixed rate. Will convert it on this end
		memcpy(&diff, msg->msg + 7, 4); //Move beyond the ADJUST- and get the integer
		memcpy(&gamma, msg->msg + 11, 4); //Move beyond ADJUST- and the first integer
		diff = abs(diff);
		float tau = pkspc - gamma;
		//Now adjust pkspc based off of the passed information and mode
		switch(mode){
			case 0:
				pkspc = pkspc + 1; //Increase by a linear factor
				break;
			case 1:
				pkspc = pkspc*delta1; 
				break;
			case 2:
				pkspc = pkspc + .001*((float)diff);
				break;
			case 3:
				pkspc = pkspc + .0001*((float)diff) - .015*tau;
				break;
			default:
				pkspc = pkspc; //Do nothing
				break;
		}
	
	}else if(strncmp(msg->msg, "ADJUST+", 7)== 0 ){
		//We have message telling us we need to more often
		
		//Pull out the data that the client sent to us
		int diff;
		int gamma; //They should transfer the fixed rate. Will convert it on this end
		float nwlambda;
		memcpy(&diff, msg->msg + 7, 4); //Move beyond the ADJUST- and get the integer
		memcpy(&gamma, msg->msg + 11, 4); //Move beyond ADJUST- and the first integer
		diff = abs(diff);
		float tau = pkspc - gamma;
		switch(mode){
			case 0:
				pkspc = pkspc - 1; //Increase by a linear factor
				break;
			case 1:
				pkspc = pkspc - 1.5; 
				break;
			case 2:
				pkspc = pkspc - .0001*((float)diff);
				break;
			case 3:
				pkspc = pkspc - .0001*((float)diff) + .015*tau;
				break;
			default:
				pkspc = pkspc; //Do nothing
				break;
		}
	}
	if(pkspc <= 0){
		pkspc = 1; //We never want it to actually go to 0
	}
}
