#include "AudioServer.h"


//Kill a zombie child process
void killzombie(int sig, siginfo_t *i, void *unused){
	//Kill the zombie process
	while(waitpid(-1, NULL, WNOHANG) > 0){
	}
}


void main(int argc, char** argv){
	//check that we have enough arguments
	if(argc < 7){
		printf("Not enough argumentes\n");
		return;
	}

	//Variables
	tcpport = atoi(argv[1]);
	udpport = atoi(argv[2]);
	udpports = argv[2];
	pyldsize = atoi(argv[3]);
	pkspc = atoi(argv[4]);
	mode = atoi(argv[5]);
	logfile = argv[6];
	audiolog = fopen(logfile, "w");
	int len = sizeof(struct sockaddr_in);
	printf("Creating server with parameters: %d %d %d %f %d %s\n", tcpport, udpport, pyldsize, pkspc, mode, logfile);

	
	//Set up child,alarm, and io handlers
	struct sigaction chld, io, alrm;
	//Make the function killzombie the child handler
	chld.sa_sigaction = &killzombie;
	chld.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &chld, NULL);
	//Make servercontrol the io handler
	io.sa_sigaction = &servercontrol;
	io.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &io, NULL);
	//Make sendpacket the alarm handler
	alrm.sa_sigaction =&sendpacket;
	alrm.sa_flags = SA_SIGINFO;
	sigaction(SIGALRM, &alrm, NULL);
	//Set the status to SETUP
	status = SETUP;

	//create the server to be used
	struct asserver* serv = createserverTCP(tcpport);	
	if(serv == NULL){
		int err = errno;
		printf("Error while creating server: %s\n", strerror(err));
		return;
	}
	


	if(handlerequest(serv, udpports) < 0){
			printf("Invalid request\n");
			close(serv->sock);
			return;
	}
	//We had a valid request and need to fork to a worker process

	//close the old server socket
	close(serv->sock);
	//create a new udp server
	printf("Creating UDP server\n");
	tserv = createserverUDP(udpport);
	//We now set the socket to nonblocking so we can receive messages from the 
	//client to control how the flow of data
	
	//We need to set up the socket to be non-blocking
	fcntl(tserv->sock, F_SETOWN, getpid());
	fcntl(tserv->sock, F_SETFL, O_NONBLOCK | O_ASYNC);

	//Open the file to read from
	file = fopen(serv->msgbuf, "r");
	//Now transfer the file
	gettimeofday(&start, NULL);
	transferfile(tserv);
	fclose(file);
	fclose(audiolog);
	close(tserv->sock);
	

	
	return;
}

