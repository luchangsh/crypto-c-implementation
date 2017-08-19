#include "histo.h"
void histo(int period, int which, FILE *ifp)
{
	int i; /* index */
	int count[CNT_SIZE];  /* Count array */
	for (i = 0; i < CNT_SIZE; i++) {
		count[i] = 0;  /* Initialize all elements to zero */
	}
	/* Count the letter frequency */
	int ch, idx = 1, L = 0;
	while ((ch = fgetc(ifp)) != '\n') {
		if ((idx - which) % period == 0 && (ch >= 'a' && ch <= 'z')) {
			count[ch - 'a']++;
			L++;
		}
		idx++;
	}
	/* Find the maximum count */
	int max = -1;
	for (i = 0; i < CNT_SIZE; i++) {
		max = (max > count[i]) ? max : count[i];
	}
	/* Print Length */
	fprintf(stdout, "L=%d\n", L);
	/* Print sorted results */
	int cnt;
	for (cnt = max; cnt >= 0; cnt--) {
		for (i = 0; i < CNT_SIZE; i++) {
			if (count[i] == cnt) {
				fprintf(stdout, "%c: %d (%.2f%%)\n", 'a'+i, count[i], ((double) count[i] / L) * 100);
			}
		}
	}
}
