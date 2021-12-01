<h1> Prova SISTEMI OPERATIVI del 21 Dicembre 2020 D </h1>

Si realizzi in linguaggio C/C++ un’applicazione basata su <b>code di messaggi UNIX e thread</b>, che
simuli la lettura di dati da parte di un sensore, e la distribuzione dei dati a più destinazioni. Un processo
<b>sensore</b> dovrà periodicamente (ogni secondo) generare un valore numerico casuale, tra 0 e 10, e
inviarlo tramite una prima coda di messaggi. Un processo <b>aggregatore</b> dovrà ricevere i dati, e inoltrare
una copia a tutti e tre i processi <b>collettori</b>, attraverso delle code di messaggi riservate ad ogni collettore.

Il processo aggregatore dovrà includere 4 thread. Il thread <b>scrittore</b> dovrà prelevare i messaggi dal
processo sensore, e scrivere il valore ricevuto su una variabile condivisa tra i thread. I thread <b>lettori</b>
dovranno leggere periodicamente (ogni secondo) il contenuto della variabile condivisa, e inviarlo ai
processi collettori. Come nello schema generale dei lettori-scrittori, i thread lettori devono poter
<b>leggere in concorrenza</b> la variabile condivisa.

Un processo padre dovrà creare gli altri processi attraverso <b>fork()</b>. Il processo sensore dovrà generare
10 messaggi in un ciclo, attendendo 1 secondo tra le iterazioni. Il thread scrittore effettuerà una
scrittura ad ogni messaggio. I thread lettori dovranno leggere la variabile condivisa per 10 volte in un
ciclo, attendendo un secondo tra le iterazioni, ed inviando altrettanti messaggi ai collettori. A loro
volta, i collettori faranno 10 ricezioni, stampando a video i valori ricevuti, per poi terminare.

<img width="1000" alt="Cattura" src="https://user-images.githubusercontent.com/95299390/144229254-725aeeaf-2698-4639-8988-9116198e9462.png">
