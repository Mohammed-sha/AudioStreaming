#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#define CLEAR 10 //Status number that lets the server know it is okay to send
#define SETUP 15 //Status number that means the server is in setup mode
#define TRANSFERING 20 //No current status for the server to be aware of
#define END 25


//Data structures that are used through out the AudioServer program
struct asserver{
	struct sockaddr_in server, client; //structures that hold informstion for client and server
	int sock; 			   //socket 
	int port;			  // port
	char* msgbuf;			  //only used for control messages between client and server
};

struct datamessage{
	char* msg;
	int size;
};

//public variables

int tcpport; //TCP port to be used. Only for intial connections
int udpport; //Port of the udp server. Used for transfering the file
char* udpports; //String representation of the above. This is easier to send to the client
int pyldsize; //Size of each payload to send to the user
float pkspc;    //Amount of time between packets
int mode;     //The prefered algorithm for sending packets
char* logfile;    //Log file of all changes to spacing 
int status; //Status of the server. CLEAR means that it is safe to send.
struct asserver* tserv;
FILE* file, *audiolog; //the file we need to read from
int mxpcksize; //Max packet size
struct timeval start;
//create a server structure to use. The passed port should be the port to accept connections on
struct asserver* createserverTCP(int port);
struct asserver* createserverUDP(int port);
//Get a connection message from a potential client
int handlerequest(struct asserver* server, char* udpport);
//Send the file over the connection
void transferfile(struct asserver* server);
//Send a message over a UDP connection
void UDPsend(struct asserver* server, struct datamessage* msg);
//Receive over a UDP connection
struct datamessage* UDPrecv(struct asserver* server, int plsize);
//SIGIO handler reads in data. This is the sigio handler
//it will update the UDP transfer only
void servercontrol(int sig, siginfo_t *i, void *unused); 
//Alarm handler
void sendpacket(int sig, siginfo_t *i, void *unused);
