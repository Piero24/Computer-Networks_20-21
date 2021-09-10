Un cookie HTTP (cookie web, browser cookie) è una piccola pezzo di dati che un server invia al browser web dell'utente. Il browser può memorizzarlo e rinviarlo alle successive richieste verso lo stesso server. In genere, viene utilizzato per stabilire se due richieste provengono dallo stesso browser, per esempio per mantenere un utente connesso.

![Funzionamento di cookie](cookie.png "Cookie")

In particolare l'header (o gli header in caso di cookie multipli) che invia il server è `Set-Cookie` che accetta una coppia `name=value` e separato da un dot-comma alcune proprietà come `Domains`, `Path`, `Expires`, ... Un esempio è:
```
Set-Cookie: LSID=DQAAAK…Eaem_vYg; Domain=docs.foo.com; Path=/accounts; Expires=Wed, 13-Jan-20021 22:23:01 GMT; Secure; HttpOnly
Set-Cookie: HSID=AYQEVn….DKrdst; Domain=.foo.com; Path=/; Expires=Wed, 13-Jan-20021 22:23:01 GMT; HttpOnly
Set-Cookie: SSID=Ap4P….GTEq; Domain=.foo.com; Path=/; Expires=Wed, 13-Jan-20021 22:23:01 GMT; Secure; HttpOnly
```

L'header che invia il client è `Cookie` anche in questo caso viene inviata una lista di `name=value` separate  da comma. Un esempio è:
```
Cookie: yummy_cookie=choco; tasty_cookie=strawberry
```
