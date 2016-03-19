The version of linux the program was originally in supported writing directly to audio through /dev/audio. Newer versions of linux do no support this
,but to get aroud it simply use ./testclient.sh | padsp tee /dev/audio > /dev/null. This causes the quality of the sound being
played to be much lower.

This will be modified in the future to actually use the proper method of playing audio.
