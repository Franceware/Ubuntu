#DEBUG = -DDEBUG_

all: meteo

meteo: meteo.o StazioneMeteo.o MonitorHoare.o
	gcc -o meteo meteo.o StazioneMeteo.o MonitorHoare.o

meteo.o: meteo.c StazioneMeteo.h
	gcc -c -o meteo.o meteo.c

StazioneMeteo.o: StazioneMeteo.c StazioneMeteo.h
	gcc -c -o StazioneMeteo.o StazioneMeteo.c

MonitorHoare.o: MonitorHoare.c MonitorHoare.h
	gcc -c -o MonitorHoare.o $(DEBUG) MonitorHoare.c

clean:
	rm -rf *.o
	rm -rf meteo
