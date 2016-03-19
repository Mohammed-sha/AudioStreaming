#include "AudioClient.h"


char* serveraddr = "192.168.1.20";
void main(int argc, char** argv){
	//Make sure we have enough arguments
	if(argc < 9){
		printf("Not enough arguments exiting\"n");
		return;
	}
	
	//Variables	
	int tport;
	tcpport = atoi(argv[1]); //port to make the initial connection on
	pyldsize = atoi(argv[2]); //palyload size
	pbdelay = atoi(argv[3]);  //play back initial delay
	pbrate  = atoi(argv[4]);	//fixed playback rate
	maxbuf  = atoi(argv[5])*1000; //maxbuffer size to keep
	targbuf = atoi(argv[6])*1000; //target buffer size multiply it by 1000 to get it into bytes
	logfile = argv[7]; //file to store buffer changes
	rqfile = argv[8];
	cl = createclientTCP((char*) serveraddr, tcpport); //Create the TCP client
	audiolog = fopen(logfile, "w");
	stream = 0;
	bflvl = 0;
	//Get memory from the heap for the buffer
	databuffer = (char*) malloc(sizeof(char)*maxbuf); //We set it to max buffer. The maxbuf should be passed in kilobytes	
	bzero(databuffer, maxbuf);	
	//intialize the semaphore
	sem_init(&lock, 0, 0); //Intialize the semaphore to have nothing it will be incremented when it has data

	printf("Creating client with parameters: %d %d %d %d %d %d\n", tcpport, pyldsize, pbdelay, pbrate, maxbuf, targbuf);



	//create a client and try to connect to the server
	printf("Sending request:%s\n", rqfile);
	if((tport = requestfile(cl, rqfile)) < 0){
		printf("Invalid file exiting..\n");
		return;
	}
	//Close the old socket we don't need it anymore
	close(cl->sock);
	free(cl);


	//we have the port now lets recreate the client
	cl = createclientUDP((char*) serveraddr, tport );

	//We have created the UDP client so now we can set up the signal handlers for SIGALRM and SIGPOLL/SIGIO
	struct sigaction alrm, io;
	//Create the alrm handler
	alrm.sa_sigaction = &playback;
	alrm.sa_flags = SA_SIGINFO;
	//Create the SIGIO handler
	io.sa_sigaction = &receivedata;
	io.sa_flags = SA_SIGINFO;
	//Now set them
	sigaction(SIGALRM, &alrm, NULL);
	sigaction(SIGIO, &io, NULL);

	//We need recvfrom to be non-blocking since we don't know when we will receive data
	fcntl(cl->sock, F_SETOWN, getpid());
	fcntl(cl->sock, F_SETFL, O_NONBLOCK | O_ASYNC);

	//Now start copying the file into memory
	//streamaudiofile(cl, pyldsize);
	/*
	audio = open("/dev/dsp", O_WRONLY);
	if(audio < 0){
		printf("%s\n", strerror(errno));
	}*/
	//Give the server the all clear
	struct datamessage msg;
	//Let the server know we are ready to start
	msg.msg = "CLEAR";
	msg.size = 6;
	stream = 1;
	UDPsend(cl, &msg); //Send the all clear
	printf("Sent all clear\n");	

	
	gettimeofday(&start, NULL);
	alarm(pbdelay); //set an alarm to playback the data
	while(stream){ //Do nothing while we are streaming
	}
	close(audio);
	fclose(audiolog);
	sem_close(&lock);
	return;
}


