#include "AudioClient.h"

int curlvl = 0;

void playback(int sig, siginfo_t* i, void* unused){
	//Simply play the audiofile.
	if(sem_trywait(&lock) != 0){
		ualarm(pbrate*1000, 0);
		return;//we need to wait for more data
	}

	if(bflvl >= pyldsize){
		write(audio, databuffer, pyldsize); //Write a payload size
		memmove(databuffer, databuffer+pyldsize, bflvl - pyldsize);//copy the remaining data to the front
		bflvl -= pyldsize;
	}else if(bflvl < pyldsize && stream == 1){
		write(audio, databuffer, bflvl); //Write a payload size
		bflvl = 0;//we are at an empty buffer
	}else if(bflvl == 0 && stream == 0){
		return;// this closes the application
	}
	struct timeval tz; //Timezone for log file
	gettimeofday(&tz, NULL);
	fprintf(audiolog, "Time:%d.%d Buffer Level:%d\n",(int) tz.tv_sec - (int)start.tv_sec,(int) tz.tv_usec, bflvl);
	ualarm(pbrate*1000, 0);
}
