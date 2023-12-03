[![Last modified](https://img.shields.io/badge/Last%20modified-10--Aug--2021-red)](https://github.com/Piero24/F.SW16-17)
# Computer-Networks_20-21

> academic year 2020-2021 (unipd)
> 
> University of Padua

---

## Exercises in C language for the examination of computer networks


These files are the exercises required in the year in previous academic years for
the Computer Networks course 1.

Here is a collection with related solutions.

The exercises consist in the implementation by the user of some pieces of code
that allow you to implement new functions.

There are the texts of the various exercises with relative solution.

**NOTE:** The exercises can have multiple solutions and not necessarily those present
in these files are the best.


## Disclaimer


These exercises are to be used ONLY to learn the basics
of computer networks.


**I AM IN NO WAY RESPONSIBLE FOR IMPROPER USE OF THIS MATERIAL.**


**DO NOT** rely solely on the following exercises for preparation.
As the course program may vary over the years.
Use this material only and exclusively for practice.


## Description


There are several server-side and client-side exercises in C language.

The basic files provided by the teacher are those called standardCW.c, standardCW.c and standardWP.c
Inside each folder there is the complete solution and / or the file with the text.

The 3 standard files have very simple comments on all the code so as to allow for easy understanding

The RDC21-prof folder contains everything you did during the year with the teacher.

The solutions instead have only the lines modified and / or added with the comment.
The other lines that are the same as those of the standard files do not have comments to make them easier to read.

### Tips

Open the terminal and type: `vim ~/.vimrc`

Copy and paste the following code

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
**RFCs useful for the exam:**

- RFC.791
- RFC.792
- RFC.793
- RFC.826
- RFC.1945
- RFC.2616

**COPY FILES TO AND FROM THE UNIPD SERVER:**

`scp <source> <destination>`

To copy the file from B to A if logged into B:

`scp /path/to/file username@a:/path/to/destination`

To copy the file from B to A if logged in to A:

`scp username@b:/path/to/file /path/to/destination`

(ad -r after scp if I want to copy a directory `scp -r <source> <destination>`)

**KEEP THE SSH CONNECTION ACTIVE**
`ssh -o ServerAliveInterval=20 username@address`



### Authors and Copyright

Pietrobon Andrea

### Note

This material will **NOT** be updated in the future.
