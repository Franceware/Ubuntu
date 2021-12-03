#DEBUG = -DDEBUG_

all: meteo

meteo: meteo.o StazioneMeteo.o MonitorSignalReturn.o
	gcc -o meteo meteo.o StazioneMeteo.o MonitorSignalReturn.o

meteo.o: meteo.c StazioneMeteo.h
	gcc -c -o meteo.o meteo.c

StazioneMeteo.o: StazioneMeteo.c StazioneMeteo.h
	gcc -c -o StazioneMeteo.o StazioneMeteo.c

MonitorSignalReturn.o: MonitorSignalReturn.c MonitorSignalReturn.h
	gcc -c -o MonitorSignalReturn.o $(DEBUG) MonitorSignalReturn.c

clean:
	rm -rf *.o
	rm -rf meteo
