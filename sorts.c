#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/*  This program benchmarks the performance of selection sorts,
 *  counting sorts and the standard library's [qsort] (which may
 *  or may not be a quick sort).
 *
 *  It's an utility that I built for a blogpost:
 *    https://cozis.github.io/post/complessita-computazionale.html
 *  about computational complexity. Since the blogpost is in 
 *  italian, the code is commented in italian too.
 *
 * (It's italian from here)
 * ------------------------------------------------------------------ 
 *  Questo programma calcola le performance di selection sort,
 *  counting sort e del [qsort] della libreria standard (che potrebbe
 *  essere o meno un quick sort).
 *
 *  È un'utilità che ho stritto per un blogpost:
 *    https://cozis.github.io/post/complessita-computazionale.html
 *  sulla complessità computazionale.
 *
 *  Questo programma implementa un selection sort e counting sort e li
 *  esegue su un array di date dimensioni per poi riportare i loro
 *  tempo di esecuzione in formato JSON. È possibile effettuare più
 *  iterazioni di ordinamento e ritornare la media di questi per ridurre
 *  la varianza dei risultati.
 *
 *  Per compilare, si può usare il comando:
 *     gcc sorts.c -o sorts -Wall -Wextra
 *
 *  Mentre per utilizzarlo, la sintassi è una di:
 *    ./sorts N
 *    ./sorts N M
 *  dove N è la dimensione degli array da ordinare ed M è il numero di
 *  ordinamenti da fare, i cui risultati saranno mediati nel risultato
 *  finale. Se il numero di iterazione M è omesso, allora si usa 1.
 *
 *  È anche possibile usare lo script "sorts.py" per chiamare questo 
 *  programma con vari parametri differenti.
 */

/* Ritorna [true] se l'array è ordinato, 
 * altrimenti [false] 
 */
bool check_array_sorting(int *array, int size)
{
	assert(array != NULL && size >= 0);

	for (int i = 0; i < size-1; ++i)
		if (array[i] > array[i+1])
			return false;
	return true;
}

void print_array(int *array, int size, FILE *fp)
{
	assert(array != NULL && size >= 0);

	fprintf(fp, "[");
	for (int i = 0; i < size; ++i) {
		fprintf(fp, "%3d", array[i]);
		if (i+1 < size)
			fprintf(fp, ", ");
	}
	fprintf(fp, "]\n");
}

/* Ritorna [true] se gli array sono uguali, 
 * altrimenti [false] 
 */
bool array_match(int *arr1, int *arr2, int size)
{
	assert(arr1 != NULL && arr2 != NULL && size >= 0);

	for (int i = 0; i < size; ++i)
		if (arr1[i] != arr2[i])
			return false;
	return true;
}

/* Ritorna l'indice nell'[array] del valore 
 * col valore più piccolo. Se l'array è vuoto,
 * è ritornato -1.
 */
int find_lowest(int *array, int size)
{
	assert(array == NULL && size >= 0);

	if (size == 0)
		return -1;

	int min_pos = -1;
	for (int i = 0; i < size; ++i)
		if (min_pos < 0 || array[i] < array[min_pos])
			min_pos = i;

	return min_pos;
}

void swap(int *a, int *b)
{
	int t = *a; *a = *b; *b = t;
}

void selection_sort(int *array, int size)
{
	/* Il selection sort funziona dividendo l'array
	 * in una prima regione ordinata ed una ancora
	 * disordinata. La regione ordinata scresce da
	 * sinistra ed è inizialmente vuota. L'ordinamento
	 * avviene in più iterazione. Ciascuna iterazione
	 * trova l'elemento più piccolo della regione
	 * disordinata e lo sposta alla fine della regione
	 * ordinata, incrementandone la dimensione e
	 * diminuendo quella della regione disordinata.
	 * Naturalmente si avrà che per ordinare ciascun
	 * elemento ci vorranno un numero di iterazioni
	 * pari al numero di elementi. Ciascun iterazione 
	 * contiene a sua volta una ricerca nella regione 
	 * disordinata, quindi la complessità computazionale
	 * sarà approssimativamente O(n^2).
	 */
	for (int i = 0; i < size; ++i) {

		/* Trova l'elemento più piccolo nella
		 * seconda metà dell'array, cioè quella
		 * ancora non ordinata che va dall'indice
		 * [i] fino a [size-1].
		 */
		int min_pos = find_lowest(array + i, size - i);

		/* Non ritorna un valore negativo
		 * perchè sappiamo che l'array non
		 * è vuoto.
		 */
		assert(min_pos >= 0);

		/* Rendiamo l'indice dell'elemento relativo
		 * all'intero array e non solo alla parte non
		 * ancora ordinata.
		 */
		min_pos += i;

		/* Metti l'elemento trovato nella ragione
		 * ordinata dell'array.
		 */
		swap(array + i, array + min_pos);
	}
}

void counting_sort(int *array, int size, int min_value, int max_value)
{
	/* Il counting sort funziona iterando lungo l'array
	 * da ordinare e contando le occorrenze dei vari valori
	 * che assumono gli elementi dell'array. Per questo 
	 * motivo è necessario un array di contatori di dimensioni
	 * pari ai possibili valori assumibili da ciascun elemento
	 * dell'array.
	 *
	 * Dato che questo è solo un esempio, assumiamo che il
	 * massimo numero di valori distinti che possono assumere
	 * gli elementi sia 32, in modo da poter usare un array 
	 * statico. Se questa assunzione non risulta verificata, 
	 * il programma viene abortito.
	 */

	// Il numero di contatori necessari.
	int counter_count = (max_value - min_value + 1);

	int counters[32];

	// Ci assicuriamo che il buffer statico sia abbastanza 
	// grande.
	if (counter_count > (int) (sizeof(counters)/sizeof(counters[0]))) {
		fprintf(stderr, "L'array statico è troppo piccolo!\n");
		abort();
	}

	// I contatori vanno inizializzati a zero!
	for (int i = 0; i < counter_count; ++i)
		counters[i] = 0;

	// Iteriamo lungo l'array e contiamo le occorrenze.
	for (int i = 0; i < size; ++i) {

		int value = array[i];
		assert(value >= min_value 
			&& value <= max_value);

		// Il contatore del valore [value] è quello
		// con indice [value - min_value].
		counters[value - min_value]++;
	}

	// Ora iteriamo lungo i contatori e per ciascuno
	// mettiamo un numero di copie del valore relativo
	// al contatore pari al valore del contatore stesso.
	for (int i = 0, w = 0; i < counter_count; ++i)
		for (int j = 0; j < counters[i]; ++j)
			array[w++] = i + min_value;
}

/* Questa è la callback di [qsort]. Se il
 * C avesse le lambda, questa ne sarebbe una.
 */
static int cmpfunc(const void *a, const void *b) 
{
   return *(int*) a - *(int*) b;
}

void quick_sort(int *array, int size)
{
	qsort(array, size, sizeof(int), cmpfunc);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		fprintf(stderr, "Dimensione dell'array mancante!\n");
		return -1;
	}

	/* --------------------------------- */
	/* -- Variabili di configurazione -- */
	/* --------------------------------- */

	// NOTA: Per semplicità non consideriamo gli errori 
	//       della conversione delle stringhe ad interi. 
	//       In caso di errore viene ritornato 0.

	int size = atoi(argv[1]); // Ignora gli errori per ora.

	// Valore minimo e massimo assumibile dagli elementi
	// degli array.
	int min_value = 0;
	int max_value = 30;

	// Numero di ordinamenti da fare (il risultato è la
	// media di tutte le iterazioni). Aumentare questo
	// numero riduce la varianza del risultato.
	int iteration_count;
	if(argc > 2) iteration_count = atoi(argv[2]);
	else         iteration_count = 1;

	// Se questa è [true], verrà stampata una percentuale
	// di caricamento con il tempo mancante. Gli indicatori
	// del progresso sono aggiornati fra le iterazioni, 
	// quindi è più utile per ordinamenti veloci in tante
	// iterazioni che per ordinamenti di array grandi con
	// poche iterazioni.
	bool print_progress = true;

	// Se questo è [true], i risultati degli ordinamenti
	// verranno controllati. Questo può essere usato per
	// testare che le funzioni funzionino correttamente,
	// tuttavia rallenta le iterazioni.
	bool check_results = false;

	fprintf(stderr, "Configuration:\n");
	fprintf(stderr, "\tMinimum value: %d\n", min_value);
	fprintf(stderr, "\tMaximum value: %d\n", max_value);
	fprintf(stderr, "\tArray size...: %d\n", size);
	fprintf(stderr, "\tIterations...: %d\n", iteration_count);
	fprintf(stderr, "\n");

	// Seme del generatore di numeri casuali. Nel 
	// caso di errori potrebbe avere senso rendere 
	// la generazione deterministica sostituendo 
	// [time(NULL)] ad un valore deterministico.
	srand(time(NULL));

	/* ---------------------------------- */
	/* -------- Inizializzazione -------- */
	/* ---------------------------------- */

	// Alloca i buffer che conterranno gli array.
	int *array,   *array_s,
	    *array_c, *array_q;

	// Possono essere allocati in una botta sola.
	void *memory = malloc(sizeof(int) * size * 4);
	if (memory == NULL) {
		fprintf(stderr, "Allocazione fallita!\n");
		abort();
	}

	array = memory;
	array_s = array   + size;
	array_c = array_s + size;
	array_q = array_c + size;

	double total_time_selection_sort = 0;
	double total_time_counting_sort  = 0;
	double total_time_quick_sort     = 0;
	double total_iteration_time = 0;

	/* --------------------------------- */
	/* ----------- Benchmark ----------- */
	/* --------------------------------- */

	int iteration_index;
	for (iteration_index = 0; iteration_index < iteration_count; ++iteration_index) {

		// Teniamo traccia di inizio e fine di ciascuna iterazione
		// per stimare il tempo rimanente prima di completare le
		// iterazioni.
		clock_t iter_begin = clock();

		// Stima e stampa il progresso ed il tempo mancante.
		if (print_progress) {
			double percent = ((double) iteration_index / iteration_count) * 100;

			double average_iteration_time = total_iteration_time / (iteration_index + 1);
			int remaining_iterations = iteration_count - iteration_index;
			double remaining = average_iteration_time * remaining_iterations;

			char timeText[32];
			if (remaining >= 60)
				snprintf(timeText, sizeof(timeText), "%d minutes", (int) (remaining / 60));
			else
				snprintf(timeText, sizeof(timeText), "%d seconds", (int) remaining);

			fprintf(stderr, "\rProgress: %.2lf%% - Remaining: %s", percent, timeText);
			fflush(stderr);
		}

		// Genera l'array da ordinare in modo casuale.
		for (int i = 0; i < size; ++i) {

			// Genera un numero compreso tra [min_value] 
			// e [max_value].
			int value = rand() % (max_value - min_value + 1) + min_value;
			assert(value >= min_value && value <= max_value);

			// Costruiamo più copie dell'array generato
			// perchè le funzioni di sorting ordinano
			// gli array modificandoli.
			array[i] = value;
			array_s[i] = value;
			array_c[i] = value;
			array_q[i] = value;
		}

		// Ordina gli array e calcola il tempo impiegato.

		double time_selection_sort; // Queste rappresentano il tempo impiegato
		double time_counting_sort;  // per ordinare in secondi per ciascun
		double time_quick_sort;     // algoritmo.

		clock_t begin, end;

		begin = clock();
		selection_sort(array_s, size);
		end = clock();
		time_selection_sort = (double) (end - begin) / CLOCKS_PER_SEC;

		begin = clock();
		counting_sort(array_c, size, min_value, max_value);
		end = clock();
		time_counting_sort = (double) (end - begin) / CLOCKS_PER_SEC;

		begin = clock();
		quick_sort(array_q, size);
		end = clock();
		time_quick_sort = (double) (end - begin) / CLOCKS_PER_SEC;

		// Aggiunti il tempo impiegato per questo ordinamento al
		// tempo totale impiegato ad ordinare rispetto all'inizio
		// del programma.
		total_time_selection_sort += time_selection_sort;
		total_time_counting_sort  += time_counting_sort;
		total_time_quick_sort     += time_quick_sort;

		// Nel caso in cui le funzioni di ordinamento non si assumano
		// corrette, controlliamo i loro risultati.
		if (check_results) {
			if (!check_array_sorting(array_q, size)) {
				fprintf(stderr, "Quick sort failed:\n");
				fprintf(stderr, "\tInput...: "); print_array(array,   size, stderr);
				fprintf(stderr, "\tOutput..: "); print_array(array_q, size, stderr);
				break;
			}

			if (!array_match(array_s, array_q, size)) {
				fprintf(stderr, "Selection sort failed:\n");
				fprintf(stderr, "\tInput...: "); print_array(array,   size, stderr);
				fprintf(stderr, "\tOutput..: "); print_array(array_s, size, stderr);
				fprintf(stderr, "\tExpected: "); print_array(array_q, size, stderr);
				break;
			}

			if (!array_match(array_c, array_q, size)) {
				fprintf(stderr, "Counting sort failed:\n");
				fprintf(stderr, "\tInput...: "); print_array(array,   size, stderr);
				fprintf(stderr, "\tOutput..: "); print_array(array_c, size, stderr);
				fprintf(stderr, "\tExpected: "); print_array(array_q, size, stderr);
				break;
			}
		}

		// Calcola quanto tempo c'è voluto a completare 
		// questa iterazione.
		clock_t iter_end = clock();
		double iteration_time = (double) (iter_end - iter_begin) / CLOCKS_PER_SEC;
		total_iteration_time += iteration_time;
	}

	if (print_progress) {
		fprintf(stderr, "\rProgress: 100.00%% - Remaining: 0.00s\n");
	}

	/* --------------------------------- */
	/* -- Output e deinizializzazione -- */
	/* --------------------------------- */

	free(memory);

	// Se è stato possibile effettuare almeno un'iterazione prima di
	// un errore, riporta i risultati dei benchmark.
	if (iteration_index > 0) {

		double average_time_selection_sort = total_time_selection_sort / iteration_index;
		double average_time_counting_sort  = total_time_counting_sort  / iteration_index;
		double average_time_quick_sort     = total_time_quick_sort     / iteration_index;

		fprintf(stdout, "{\"size\": %d, \"iterations\": %d, \"selection-sort\": %lf, \"counting-sort\": %lf, \"quick-sort\": %lf}",
			size, iteration_count, average_time_selection_sort, average_time_counting_sort, average_time_quick_sort);
		/*
		fprintf(stdout, "Average times:\n");
		fprintf(stdout, "\tSelection sort: %lfs\n", average_time_selection_sort);
		fprintf(stdout, "\tCounting sort.: %lfs\n",  average_time_counting_sort);
		fprintf(stdout, "\tQuick sort....: %lfs\n",     average_time_quick_sort);
		*/
		return 0;
	} else {
		fprintf(stdout, "Couldn't evaluate average times.\n");
		return -1;
	}
}