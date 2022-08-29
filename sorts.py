import json
import subprocess
from os.path import exists

"""
	This program benchmarks the performance of selection sorts,
 	counting sorts and the standard library's [qsort] (which may
 	or may not be a quick sort). It works by calling "sorts.c"
 	iteratively.

 	It's an utility that I built for a blogpost:
 		https://cozis.github.io/post/complessita-computazionale.html
 	about computational complexity. Since the blogpost is in 
 	italian, the code is commented in italian too.
 	
 	(It's italian from here)
 	------------------------------------------------------------------ 
 	Questo programma calcola le performance di selection sort,
 	counting sort e del [qsort] della libreria standard (che potrebbe
 	essere o meno un quick sort). Funziona chiamando "sorts.c"
 	iterativamente.
 	
 	È un'utilità che ho stritto per un blogpost:
 		https://cozis.github.io/post/complessita-computazionale.html
 	sulla complessità computazionale.

 	In pratica chiama "sorts.c" con dimensioni di array che sono
 	multiple di [step_size], partendo con [min_size] e finendo con
 	[max_size]. Alla fine i risultati sono salvati in un file chiamato
 	"sort_data_N.json", dove N è un numero determinato per non
 	sovrascrivere file generati precedentemente.

 	Se il programma è interrotto da un'eccezione, i risultati parziali
 	vengono comunque salvati nel file. Per questo motivo è possibile
 	concludere l'esecuzione con ctrl+C in modo sicuro.
"""

min_size = 10
max_size = 1_000_000
step_size = 10

def choose_output_file_name():
	i = 0
	while exists(name := f"sort_data_{i}.json"):
		i += 1
	return name

table = []
try:
	for n in range(min_size, max_size, step_size):

		# Indipendentemente dalla dimensione dell'array
		# si usa un numero di iterazioni pari a 100.
		# Per ottimizzare le performance del programma
		# sacrificando un po' di precisione, si potrebbe
		# fare un modo che il numero di iterazioni sia
		# grande per array piccoli e piccolo per array
		# grandi.
		iter_no = 100

		res = subprocess.run(["./sorts", str(n), str(iter_no)], 
			                 capture_output=True, encoding='utf-8')

		if res.returncode == 0:
			# Completed succesfully
			text = res.stdout
			data = json.loads(text)
			table.append(data)
			print(data)
		else:
			# Failed
			error = res.stderr
			raise error
except:
	name = choose_output_file_name()

	# Write table to file
	with open(name, 'w', encoding='utf-8') as f:
		json.dump(table, f, ensure_ascii=False, indent=4)