Esame di Reti di Calcolatori -  30 Agosto 2021

Si modifichi il programma ​esame.c che implementa il Web Server sviluppato durante il corso, in modo tale che sia in grado di gestire il meccanismo di controllo del caching basato sull’ETag descritto nella ​RFC2616. Capitoli 3.11, 14.19, 14.24, 14.26.

Al posto di utilizzare la data per stabilire se una risorsa è modificata il server associa un identificativo comunicato con l’header “Etag” che riassume il contenuto del file e che rimane invariato se e solo se il contenuto della risorsa non varia.

Si provi con un browser l’accesso ripetuto ad una pagina. Il server si dovrà comportare correttamente a seconda che il file sia stato modificato o meno tra una richiesta e l’altra del client. Si implementi il minimo indispensabile per soddisfare le richieste del client.
Per connettersi al server cloud usare il comando ssh username@88.80.187.84

Scrivere (come /* commento */ all'inizio del file esame.c) una breve relazione su
1. Funzionalità puntuali da aggiungere
2. Punti di intervento neĺ programma
3. Eventuali scelte implementative
4. Descrizione dell’esecuzione e verifica correttezza.
oppure
1. /*RITIRATO*/, nel caso in cui si decida di ritirarsi
