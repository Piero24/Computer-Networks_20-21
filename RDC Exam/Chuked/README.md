Abbiamo visto che in HTTP/1.0 viene introdotto il campo Content-Lenght e sottolineato come in realtà è mal digerito dai server web. Ricapitolando:

* HTTP/0.9: la connessione finisce con la chiusura della connessione.
* HTTP/1.0: la connessione finisce con la chiusura della connessione e/o se presente l'header `Connection: Keep-Alive` (non standard) vengono conteggiati i Byte e deve essere presente la relativa `Content-Lenght`.
* HTTP/1.1: la connessione è Keep-Alive e vengono conteggiati i Byte attraverso  `Content-Lenght` oppure la nuova modalità Transfert-Encoding, che utilizza un Chunked-Body.

Il `Transfer-Encoding` è una intestazione specifica per indicare che codifica è stata usata per trasferire in modo sicuro il corpo del payload all'utente. I principali `Transfer-Encoding` sono:
```
Transfer-Encoding: chunked
Transfer-Encoding: compress
Transfer-Encoding: deflate
Transfer-Encoding: gzip
Transfer-Encoding: identity
```

Il `Transfer-Encoding: chunked` è il più utilizzato: I dati vengono inviati in una serie di blocchi, ogni blocco ci ha una sua lunghezza, l'intestazione `Content-Length` viene omessa e se presente è da ignorare. Ogni singolo blocco  (Chunked-Body) contiene una lunghezza in formato esadecimale, il `\r\n` ed il corpo del messaggio seguito da un altro `\r\n`.

Il messaggio finisce quando si arriva a blocco con  lunghezza pari a zero, è infine seguita dal trailer, che consiste in una sequenza (possibilmente vuota) di campi di intestazione dell'entità. Altre modalità utilizzano algoritmi di complessione come compress - LZW, deflate - ZLIB, gzip - LZ77, infine la modalità identity, cioè nessun encoding specifico e va trattato come se fosse HTTP/1.0.

Un Chunked-Body indica una lunghezza parziale per una parte di messaggio, nello standard RDC 2616 è definito come:
```
Chunked-Body  = *chunk
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

Cioè significa che una possibile risposta è:
```
HTTP/1.1 200 OK\r\n
Content-Type: text/plain\r\n
Transfer-Encoding: chunked\r\n
Trailer: Expires\r\n
\r\n
7\r\n
Mozilla\r\n
9\r\n
Developer\r\n
7\r\n
Network\r\n
0\r\n
Expires: Wed, 21 Oct 2015 07:28:00 GMT\r\n
\r\n
```

La gestione della lettura del Chunked-Body è la seguente:
```
length := 0
read chunk-size, chunk-extension (if any) and CRLF
while (chunk-size > 0) {
    read chunk-data and CRLF
    append chunk-data to entity-body
    length := length + chunk-size
    read chunk-size and CRLF
}
read entity-header
while (entity-header not empty) {
    append entity-header to existing header fields
    read entity-header
}
Content-Length := length
Remove "chunked" from Transfer-Encoding
```
