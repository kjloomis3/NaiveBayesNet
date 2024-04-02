
datastream.o: datastream.c datastream.h
		gcc -g datastream.c -c

stream_fncts.o: stream_fncts.c stream_fncts.h
		gcc -g stream_fncts.c -c

dataStreamMgr.o: dataStreamMgr.c dataStreamMgr.h
		gcc -g dataStreamMgr.c -c

timeval.o: timeval.c timeval.h
		gcc -g timeval.c -c

fibonacci.o: fibonacci.c fibonacci.h
		gcc -g fibonacci.c -c

multiply3.o: multiply3.c multiply3.h
		gcc -g multiply3.c -c

control.o: control.c control.h
		gcc -g control.c -c

filter_odd.o: filter_odd.c filter_odd.h
		gcc -g filter_odd.c -c

fibonacci: timeval.o datastream.o stream_fncts.c dataStreamMgr.c fibonacci.c
		gcc -g timeval.o datastream.o stream_fncts.o dataStreamMgr.o fibonacci.o -lpthread -o fib.exe

multiply3: timeval.o datastream.o stream_fncts.o dataStreamMgr.o multiply3.o
		gcc -g timeval.o datastream.o stream_fncts.o dataStreamMgr.o multiply3.o -lpthread -o multiply3.exe

simple: timeval.o datastream.o stream_fncts.o dataStreamMgr.o simple.o
		gcc -g timeval.o datastream.o stream_fncts.o dataStreamMgr.o simple.o -lpthread -o simple.exe

control: timeval.o datastream.o stream_fncts.o dataStreamMgr.o control.o
		gcc -g timeval.o datastream.o stream_fncts.o dataStreamMgr.o control.o -lpthread -o control.exe

filter_odd: timeval.o datastream.o stream_fncts.o dataStreamMgr.o filter_odd.o
		gcc -g timeval.o datastream.o stream_fncts.o dataStreamMgr.o filter_odd.o -lpthread -o filter_odd.exe

make all: 
	make fibonacci
	make multiply3
	make simple
	make control
	make filter_odd