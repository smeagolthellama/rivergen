/* RAND_MAX assumed to be 32767 */
int myrand(void)
{
	next = next * 1103515245 + 12345;
	return ((unsigned)(next / 65536) % 32768);
}

void mysrand(unsigned int seed)
{
	next = seed;
}

// quicksort_swap --- helper function for quicksort, should really be inline

void quicksort_swap(int *data, unsigned int i, unsigned int j)
{
	int temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}

void quicksort(int *data, int left, int right)
{
	int i, last;

	if (left >= right) { // do nothing if array contains fewer
		return;    // than two elements
	}

	quicksort_swap(data, left, int((left + right) / 2));
	last = left;

	for (i = left + 1; i <= right; i++)
		if (data[i] < data[left]) {
			quicksort_swap(data, ++last, i);
		}

	quicksort_swap(data, left, last);
	quicksort(data, left, last - 1);
	quicksort(data, last + 1, right);
}
