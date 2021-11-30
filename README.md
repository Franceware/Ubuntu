#Esercitazione

Si completi in linguaggio C o C++ un programma multi-thread che simuli il monitoraggio di traffico navale. Il programma dovrà essere basato sul costrutto Monitor e risolvere un problema lettori/scrittori con starvation di entrambi.

Si supponga di monitorare 5 navi, la cui posizione (il molo in cui si trova la nave) sia rappresentata da un valore intero compreso tra 0 e 10. La posizione della nave viene aggiornata da dei thread gestori del molo, e consultata da dei thread denominati viaggiatori. Ciascuna nave deve essere monitorata usando una istanza distinta del monitor così definito:

Il metodo leggi_molo() dovrà restituire la posizione attuale della nave, permettendo a più viaggiatori di leggere in contemporanea. Il metodo scrivi_molo() dovrà permettere ai gestori del molo di aggiornare la posizione della nave, garantendo la mutua esclusione tra i thread.

Il programma principale dovrà istanziare 5 istanze del monitor e 5 thread gestori del molo (una istanza e un thread per ogni nave). I gestori del molo dovranno invocare per 10 volte il metodo scrivi_molo(), modificando il valore della posizione ad ogni invocazione (incrementando il valore di 1) e attendendo 3 secondi tra le invocazioni. Il valore del molo deve essere inizialmente posto a 0.

Inoltre, dovranno essere istanziati 10 thread viaggiatori. I thread viaggiatori dovranno scegliere una nave a caso, e dovranno consultare la posizione della nave scelto per 3 volte, invocando il metodo leggi_molo() dopo avere atteso per un tempo casuale (tra 1 e 6 secondi) tra le invocazioni.
