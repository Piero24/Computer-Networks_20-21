La **basic authentication** è un metodo fornito dal protocollo HTTP per fornire credenziali di accesso tra client e server. La suddetta implementazione, a differenza delle implementazioni comuni non richiede cookie, id di sessione e pagine di login ed **è gestita dal server web** e non da uno script (PHP, Java, ...) che a sua volta viene richiamato dal server web.

![Funzionamento di un HTTP BA](auth.png "HTTP BA")

L'implementazione HTTP Basic authentication (BA)  utilizza headers HTTP statici e standard che non richiedono handshake eseguito preventivamente, il funzionamento è molto semplice: il client richiede una pagina web al server e solo nel caso venga richiesta l'autenticazione il server risponde `401 Unauthorized` con l'header `WWW-Authenticate: Basic realm="realm"` dove il realm è il titolo che appare nel popup del browser, il client chiede all'utente username e password e li invia al server. Se il browser sa già l'username e la password essa viene inviata già alla prima richiesta.

![Schermata di Login del browser per HTTP BA](auth-login.png "Schermata di Login del browser per HTTP BA")

Il client invia al browser una stringa nel formato `username:password` codificato in base64, nel caso di un username uguale a `apriti` e una password uguale a `sesamo`, il relativo base64 è `YXByaXRpOnNlc2Ftbw==` e il relativo header è `Authorization: Basic YXByaXRpOnNlc2Ftbw==`.

L'header BA deve essere inviato in ogni richiesta HTTP, il browser deve inserire nella cache le credenziali per un periodo di tempo congruo per evitare di dover richiedere ogni volta all'utente la username e la password. HTTP non fornisce un metodo al web server per indicare al browser di eseguire il logout, ci sono una serie di possibilità utilizzando specifiche caratteristiche nei differenti browser, ma le implementazioni cambiano da browser a browser.

L'header BA trova ancora ampio uso nei proxy, per esempio il proxy fornito dall'università di Padova [Auth Proxy UNIPD](https://bibliotecadigitale.cab.unipd.it/bd/auth-proxy) usa una autenticazione HTTP.

L'autenticazione BA non prevede protezione per le credenziali trasmesse. Esse vengono banalmente codificate con base64 ma non criptate o crittografate con hash (N.B. se il canale è sicuro è sconsigliato l'uso di hash lato browser in quanto il server non ha modo di fare ulteriori verifiche è l'evenutale salt è pubblico sul codice JavaScript), il problema si risolve in HTTPS in quanto pure gli header sono crittografati.

Nel caso di username/passowrd errata se il server autorizza un nuovo login è da rispondere con `401 Unauthorized`, mentre se per esempio l'utente è bannato dopo due password errate è preferibile `403 Forbidden`
