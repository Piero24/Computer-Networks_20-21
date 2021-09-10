Esame di Reti di Calcolatori - 24 Luglio 2015

Si modifichi il programma Client in modo che incorpori un meccanismo di caching delle risorse scaricate, facendo riferimento ai seguenti punti: 

1. Si utilizzi l’header Last-Modified dell’ HTTP/1.0, documentato alla sezione 10.10 della RFC 1945.

2. Ad ogni risorsa scaricata si associ un file 
    1. il cui nome corrisponde all’URL della risorsa (nel quale il carattere “/” viene sostituito dal carattere “_”)
    2. il cui contenuto è composto da 
        1. una prima riga contenente la data di download della risorsa (espressa nel modo più conveniente)
        2. il contenuto della risorsa (entity body)
    3. la cui cartella di salvataggio è ./cache/, figlia del working directory del programma proxy. 

3. Per la gestione della data 
    1. si  faccia riferimento al formato  http-date (cfr. RFC 1945, Sezione 3.3)   
    2. si utilizzino le funzioni, documentate nel manuale UNIX  nelle apposite sezioni riportate tra parentesi.  
        1. time(2) per ottenere la data espressa in secondi a partire dal 1/1/1970 (detta “Epoch” o “Unix time”) nel tipo int rinominato time_t 
        2. localtime(3) per scomporre la data espressa in “epoch” nelle sue componenti  (ora, minuti, ... etc.) riportate ciascuna in un campo della struttura struct tm  e viceversa  mktime(3) per effettuare l’operazione inversa.
        3. opzionalmente utilizzare strftime(3) per formattare (analogamente  alla printf) le componenti della data presenti nei campi della struct tm in una stringa e strptime(3)  per effettuare (similmente alla scanf) l’operazione inversa. 

4. Si proceda come segue: 
    1. si scriva sul retro del presente foglio il diagramma di flusso o il pseudo-codice corrispondente alla logica che si intende utilizzare per implementare la cache sul client 
    2. si  modifichi il programma 
    3. si utilizzi per le prove il sito web www.example.com
