[![Last modified](https://img.shields.io/badge/Last%20modified-10--Aug--2021-red)](https://github.com/Piero24/F.SW16-17)
# RDC20-21

> anno accademico 2020-2021 (unipd)
> Università degli studi di Padova

---
**Multilanguage**

*Versioni in altre lingue:* [English](https://github.com/Piero24/RDC20-21/blob/main/README-EN.md) - [Italiano](https://github.com/Piero24/RDC20-21/blob/main/README.md)

## Esercizi in linguaggio C per l'esame di reti dei calcolatori


Questi file sono gli esercizi richiesti nell'anno negli anni accademici precedenti per
il corso di Reti dei calcolatori 1.

Qui è presente una raccolta con le relative soluzioni.

Gli esercizi consistono nell'implementazione da parte dell'utente di alcune parti di codice
che permettano di implementare nuove funzioni.

Sono presenti i testi dei vari esercizi con relativa soluzione.

**NOTA:** Gli esercizi posso avere molteplici soluzioni e non è detto che quelle presenti
in questi file siano le migliori.


## Disclaimer


Questi esercizi devono essere utilizzati ESCLUSIVAMENTE per apprendere le basi
di reti dei calcolatori.


**NON SONO IN ALCUN MODO RESPONSABILE DELL'UTILIZZO IMPROPRIO DEL PRESENTE MATERIALE.**


**NON** basarsi esclusivamente sui seguenti esercizi per la preparazione.
In quanto il programma del corso potrebbe variare durante gli anni.
Usare questo materiale solo ed esclusivamente per esercitarsi.


## Descrizione


Sono svariati esercizi lato server e lato client in linguaggio C.

I file di base forniti dal docente sono quelli chiamati standardCW.c, standardCW.c e standardWP.c
All'interno di ogni Cartella è presente la soluzione completa e/o il file con il testo.

I 3 file standard hanno commenti molto semplici su tutto il codice così da permettere
una facile comprensione

Il file txt contiene alcuni consigli e personalizzazioni per il text editor vim che verrà usato all'esame.
La cartella **RDC21-prof** contiene tutto quello fatto durante l'anno con il docente. 

Le soluzioni invece hanno solo le righe modificate e/o aggiunte con il commmento.
Le altre righe uguali a quelle dei file standard non presentano commenti per facilitarne la lettura.

### Tips

Aprire il terminale e digitare: `vim ~/.vimrc`

Copiare e incollare il seguente codice

```
" F8 to save and compile creating np executable                                                                                                                                           
map <F8> :w <CR> :!gcc % -o np -g <CR>
" F9 to execute
map <F9> :!./np <CR>

" make your code look nicer
set tabstop=4
set shiftwidth=4
set softtabstop=0 expandtab
set incsearch
set cindent


" Insert line numbers
set number
" Insert the line to see which line we are in
set cursorline
" By putting 'a' you can use the cursor to move but you cannot copy paste with 'r' or 'v' you can copy paste
set mouse=r
" Set the colors for the syntax
syntax on
```

**RFC utili all'esame:**

- RFC.791
- RFC.792
- RFC.793
- RFC.826
- RFC.1945
- RFC.2616

**COPIARE FILE DA E VERSO IL SERVER UNIPD:**

`scp <source> <destination>`

Per copiare il file da B ad A se loggati in B:

`scp /path/to/file username@a:/path/to/destination`

Per copiare il file da B ad A se loggati in A:

`scp username@b:/path/to/file /path/to/destination`

(aggiungere -r dopo scp se voglio copiare una directory `scp -r <source> <destination>`)

**TENERE ATTIVA LA CONNESSIONE SSH**
`ssh -o ServerAliveInterval=20 username@address`




### Authors and Copyright

Pietrobon Andrea

### Note

Questo materiale **NON** verrà più aggiornato in futuro.
