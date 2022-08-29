# ENG
This program benchmarks the performance of selection sorts,
counting sorts and the C standard library's `qsort`.

It's an utility that I built for a blogpost:
    https://cozis.github.io/post/complessita-computazionale.html
about computational complexity. Since the blogpost is in 
italian, the code is commented in italian too.

(it's italian from here)

# ITA
Questo programma calcola le performance di selection sort,
counting sort e del [qsort] della libreria standard del C.

È un'utilità che ho stritto per un blogpost:
    https://cozis.github.io/post/complessita-computazionale.html
sulla complessità computazionale.

Per usarlo va prima compilato `sorts.c` generando un eseguibile chiamato `sort`
(il comando da usare è scritto nel file). Eseguendo `sorts.py` (con l'ausilio 
di `sort`), verrà generato un file chiamato `sort_data_N.json` contenente i 
risultati dei benchmark. Questo file può essere dato in pasto a `plot.py` per 
generare un'immagine png contenente un grafico dei risultati. I parametri degli 
script vanno modificati dal loro interno.