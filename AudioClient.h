#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>


#define SUCCESS 1
#define FAILURE -1

extern char* serveraddr;


//Struct containing all the information the client needs
struct asclient{
	struct sockaddr_in server;
	int port;
	int sock;
	char* msgbuf; //A message buffer used only to get control messages from server
};

struct datamessage{
	char* msg; //The actual message
	int size;  //Amount of data received
};

struct audiobuffer{
	char* audata; 	//Audio data
	int size;
	struct audiobuffer* next; //Next segment of data
};


//Variables
int tcpport; //port to make the initial connection on
int pyldsize; //palyload size
int pbdelay;  //play back initial delay
int pbrate;	//fixed playback rate
int maxbuf; //maxbuffer size to keep
int targbuf; //target buffer size
char* logfile; //file to store buffer changes
char* databuffer; //Playback buffer for the audio file. Should never be full, should never be empty. Point of lab
struct asclient* cl;
int bflvl; //buffer level of the databuffer
int audio; //Temporary file to store the audio file
int stream; //flag to let client know it is streaming
sem_t lock; //producer/consumer lock
struct audiobuffer* head, *tail; //Head and tail of the audiobuffer
FILE *audiolog; //Audio log of the changes sent on this end
char* rqfile; //The requested file
struct timeval start;

//Create the client
struct asclient* createclientTCP(char* address, int port);
struct asclient* createclientUDP(char* address, int port);
//Creates the socket
int tcpinit(struct asclient* cl);
//Set the tport of the client to the passed port
void settransport(struct asclient* cl, int nport);
//Send the request and get the port
int requestfile(struct asclient* cl, char* file);
//Stream the file with the passed parameters
void streamaudiofile(struct asclient* cl, int pyldsize);
//read from the socket and return the message. Sets clients dataread field
struct datamessage*  UDPrecv(struct asclient* cl, int pyldsize);
//send the message to the server
void UDPsend(struct asclient* cl, struct datamessage* msg);
//SIGALRM handler, functions as a consumer of data produced by SIGIO(receivedata)
void playback(int sig, siginfo_t* i, void* unused);
//SIGIO handler, functions as the producer of datt for SIGALRM(playback)
void receivedata(int sig, siginfo_t* i, void* unused);


