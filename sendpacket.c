#include "AudioServer.h"


//Send at every interval of pkspc
void sendpacket(int sig, siginfo_t* i, void* unused){
		if(feof(file)){
			printf("Ending\n");
			//We need to end
			//We reached the end of the file let the client know
			struct datamessage *msg = (struct datamessage*) malloc(sizeof(struct datamessage));
			msg->msg = "END";
			msg->size = 4;
			UDPsend(tserv,msg);
			status = END;
			return;
		}
		//create a datamessage to hold the data
		struct datamessage msg;
		msg.msg = (char*) malloc(sizeof(char)*pyldsize);
		//read into the buffer from the file
		msg.size = fread(msg.msg, 1, pyldsize, file);
		//Send the data to the client
		UDPsend(tserv, &msg);
		//Free the message buffer
		free(msg.msg);

		struct timeval tz; //Timezone for log file
		gettimeofday(&tz, NULL);
		fprintf(audiolog, "Time:%d.%d %f\n", (int)tz.tv_sec - (int)start.tv_sec ,(int)tz.tv_usec, pkspc);
		if(pkspc >= 1000){
			pkspc = 999;
		}
		ualarm(pkspc*1000, 0);
}
