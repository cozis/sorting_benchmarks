import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import json

"""
	This program plots the benchmark results generate by "sorts.py".
	
 	It's an utility that I build for a blogpost:
 		https://cozis.github.io/post/complessita-computazionale.html
 	about computational complexity. Since the blogpost is in 
 	italian, the code is commented in italian too.
 	
 	(It's italian from here)
 	------------------------------------------------------------------ 
 	Questo programma grafica i risultati dei benchmark generati da 
 	"sorts.py".
	
 	È un'utilità che ho stritto per un blogpost:
 		https://cozis.github.io/post/complessita-computazionale.html
 	sulla complessità computazionale.
"""

def data_to_figure(data_file, figure_file, max_size):
	""" 
		Dato un file "sort_data_N.json" generato da "sorts.py", 
		questa funzione genera un grafico a partire da quei 
		risultati e lo salva come immagine png. 

		Il parametro [figure_file] è il nome dell'immagine 
		png che verrà salvata, mentre [max_size] fa in modo
		che i risultati degli ordinamenti relativi ad array
		con dimensione uguale o maggiore di [max_size] siano
		ignorati. Se è [None] allora tutti i dati sono
		considerati.
	"""

	with open(data_file, 'r') as f:
		table = json.load(f)

	x = []
	y1 = []
	y2 = []
	y3 = []
	prev_size = None
	for row in table:
		size = row['size']
		
		assert (prev_size == None and size == 10) or (prev_size + 10 == size)

		if size >= max_size:
			break

		x.append(size)
		y1.append(row['selection-sort'])
		y2.append(row['counting-sort'])
		y3.append(row['quick-sort'])

		prev_size = size

	fig, ax = plt.subplots()

	ax.plot(x, y1, '-')
	ax.plot(x, y2, '-')
	ax.plot(x, y3, '-')

	ax.legend(['Selection sort', 'Counting sort', 'qsort'])
	plt.xlabel('Numero di elementi')
	plt.ylabel('Tempo di esecuzione (secondi)', labelpad=20)
	#plt.show()
	fig.tight_layout()
	fig.savefig(figure_file)

data_to_figure('sort_data_3.json', 'figure.png', None)