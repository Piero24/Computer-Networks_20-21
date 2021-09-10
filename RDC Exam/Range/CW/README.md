Quando si ha a che fare con file di grosse dimensioni spesso si ricorre tecnologie per segmentare la risposta HTTP in quando lo scaricamento di una risorsa di grandi dimensioni rischia di essere interrotto dalla perdita della connessione. Risulta evidente che anche ripetendo più volte lo scaricamento del file intero si rischia ogni volta l’interruzione e di conseguenza la probabilità di terminare con successo l’operazione diviene molto bassa. Per ovviare a questo inconveniente il client web dev’essere modificato in modo tale che scarichi a piccoli pezzi il file tramite una sequenza di più richieste che scaricano ciascuna un segmento del file.
Questo permette anche di effettuare download su connessioni multiple aumentando la velocità di trasmissione.

Il protocollo HTTP/1.1 supporta questa funzione per mezzo dell’header `Range` ed il relativo  header di risposta `Content-Range`. Documentato nella RFC 2616 in particolare nelle sezioni 14.35 e 14.16 e 3.12.


Il client usa l'header `Range` per specificare quanti byte vuole scaricare, per esempio:
```
Range: bytes=0-10000
```
Il server risponde con:
```
Content-Range: bytes 0-10000/67589
```

Infine tramite l'header `Accept-Ranges` è possibile specificare l'unità di misura (`bytes`) e/o `none` nel caso in cui non è accettato l'header. È anche possibile usare l'header `Range` per fare connessioni in parallelo.

È anche possibile fare richieste di più range, per esempio i bytes `0-50, 100-150`, in questo caso  i range sono seprati da comma e il server risponde attraverso un `Content-Type: multipart/byteranges` e relativo boundary, come nell'invio di file in una richiesta POST.
```
HTTP/1.1 206 Partial Content
Content-Type: multipart/byteranges; boundary=3d6b6a416f9b5
Content-Length: 282

--3d6b6a416f9b5
Content-Type: text/html
Content-Range: bytes 0-50/1270

<!doctype html>
<html>
<head>
    <title>Example Do
--3d6b6a416f9b5
Content-Type: text/html
Content-Range: bytes 100-150/1270

eta http-equiv="Content-type" content="text/html; c
--3d6b6a416f9b5--
```

Nel caso di risposta corretta viene resitituo la status line `206 Partial Content`. Nel caso in cui la richiesta non può essere soddisfatta per errori out-of-bound: `416 Requested Range Not Satisfiable`, e se il server non supporta il range: `200 OK`.
