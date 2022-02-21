<h1> Prova Maggio 2021</h1>

<p> Si realizzi in linguaggio C/C++ un programma <b>multithread</b>, basato sul <b>costrutto Monitor</b>, che realizzi
  lo schema del <b>produttore-consumatore con vettore di stato</b>. È previsto che i produttori possano inserire
elementi di due tipi differenti. I buffer del vettore possono essere in 4 possibili stati: LIBERO, IN_USO,
OCCUPATO1, OCCUPATO2 (questi ultimi due stati a seconda del tipo di elemento che è stato
prodotto). I produttori chiamano uno fra due differenti metodi ("produci_tipo_1" e "produci_tipo_2") per
effettuare la produzione sui due tipi differenti. Analogamente, i consumatori chiamano uno fra due
metodi differenti ("consuma_tipo_1" e "consuma_tipo_2") per consumare. </p>
<p> Per verificare il funzionamento del programma, si creino 2 thread produttori, che effettuino 4 produzioni
ciascuno di tipo 1; altri 2 thread produttori, che effettuino 4 produzioni ciascuno di tipo 2; 1 thread
consumatore, che effettui 8 consumazioni di tipo 1; 1 thread consumatore, che effettui 8 consumazioni
di tipo 1. Il vettore di buffer conterrà al più 4 elementi. </p>
