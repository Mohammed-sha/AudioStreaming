The version of linux the program was originally in supported writing directly to audio through /dev/audio. Newer versions of linux do no support this,but to get aroud it simply use ./myaudiostream (args) | padsp tee /dev/audio > /dev/null. This causes the quality of the sound being
played to be much lower.

This will be modified in the future to actually use the proper method of playing audio.


client: myaudiostream takes 9 arguments, myaudiostream (tcp port no.) (payload size) (initial delay) (playback rate) (maxbuffer size) (target buffer size) (logfile) (server address) (the requested audio file in .au format). myaudiostream is the client program and should be started after myaudiostreamd.

server: myaudiostreamd takes 7 arguments, myaudiostreamd (tcp port no.) (udp port no.) (pay load size) (packet spacing) (mode) (logfile). Run myaudiostreamd before myaudiostream. The server and clients tcp port number should match 

Good values to use for myaudiostreamd are, pay load size = 250, packet spacing = 90, mode = 3
Good values to use myaudiostream are payload size = 250, initial delay = 3ms, playback rate = 10, maxbuffer size = 40, target buffer size = 20

The port numbers can be whatever you wish them to be they only need to be availible to use and the tcp port numbers of both the client and server need to match.
