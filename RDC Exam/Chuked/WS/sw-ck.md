Esame di Reti di Calcolatori - 19 Giugno 2015

Si modifichi il programma che implementa il Web Server in modo che invii un entity body utilizzando il transfer coding chuncked, come descritto dalla grammatica riportata nella sezione 3.6.1 della RFC 2616, evitando tutte le parti opzionali. 
```
Chunked-Body   = *chunk
                        last-chunk
                        trailer
                        CRLF

       chunk          = chunk-size [ chunk-extension ] CRLF
                        chunk-data CRLF
       chunk-size     = 1*HEX
       last-chunk     = 1*("0") [ chunk-extension ] CRLF

       chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
       chunk-ext-name = token
       chunk-ext-val  = token | quoted-string
       chunk-data     = chunk-size(OCTET)
       trailer        = *(entity-header CRLF)

```
Per evitare interferenze si utilizzi come numero di porta le ultime 4 cifre del proprio numero di matricola. 

Si verifichi che il browser scarichi correttamente tutto l’entity body. 


