#include "hw2.h"

int main(int argc, char *argv[])
{
	FILE *fp = stdin; /* default input: stdin */
	int readFromFile = 0;
			
	argc--; argv++; /* skip argv[0] */ 
	if (argc <= 0) {
		fprintf(stderr, "Command not specified\n");
		usage(); /* exit */
	}
	if (strcmp(*argv, "stream") == 0) {
		if (--argc != 2) {
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		char *pphrase = NULL;
		int len = -1;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') {
				fprintf(stderr, "Not an option, %s\n", *argv);
				usage(); /* exit */
			}
			char *val = parseOption(*argv);
			if (val == NULL) {
				fprintf(stderr, "Malformed option, %s\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Empty value of \'%s\' option\n", *argv);
				usage(); /* exit */
			}
			if (strcmp(*argv, "-p") == 0) {
				pphrase = malloc(strlen(val) + 1);
				strncpy(pphrase, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-l") == 0) {
				len = parseInt(val);
			} else {
				fprintf(stderr, "Unknown option, %s\n", *argv);
				usage(); /* exit */
			}
		}
		if (pphrase == NULL || len == -1) {
			fprintf(stderr, "Malformed command\n");
			usage(); /* exit */			
		}
		stream(pphrase, len, NULL);
		free(pphrase);
	} else if (strcmp(*argv, "encrypt") == 0) {
		if (--argc < 2 || argc > 3) {
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		char *pphrase = NULL;
		char *name = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* argument [pbmfile] */
				readFromFile = 1;
				if ((fp = fopen(*argv, "r")) == NULL) {
					fprintf(stderr, "Can't open file %s for reading\n", *argv);
					exit(EXIT_FAILURE);
				}
			} else { /* options */
				char *val = parseOption(*argv);
				if (val == NULL) {
					fprintf(stderr, "Malformed option, %s\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Empty value of \'%s\' option\n", *argv);
					usage(); /* exit */
				}
				if (strcmp(*argv, "-p") == 0) {
					pphrase = malloc(strlen(val) + 1);
					strncpy(pphrase, val, strlen(val) + 1);
				} else if (strcmp(*argv, "-out") == 0) {
					name = malloc(strlen(val) + 1);
					strncpy(name, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Unknown option, %s\n", *argv);
					usage(); /* exit */
				}			
			}
		}
		if (pphrase == NULL || name == NULL) {
			fprintf(stderr, "Malformed command\n");
			usage(); /* exit */			
		}
		encrypt(pphrase, name, fp);
		free(pphrase);
		free(name);
	} else if (strcmp(*argv, "merge") == 0) {
		if (--argc != 2) {
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}	
		FILE *fp1 = NULL;
		FILE *fp2 = NULL;
		if ((fp1 = fopen(*++argv, "r")) == NULL) {
			fprintf(stderr, "Can't open file %s for reading\n", *argv);
			exit(EXIT_FAILURE);
		}
		if ((fp2 = fopen(*++argv, "r")) == NULL) {
			fprintf(stderr, "Can't open file %s for reading\n", *argv);
			exit(EXIT_FAILURE);
		}
		merge(fp1, fp2);
		if (fclose(fp1) != 0) {
			fprintf(stderr, "Error in closing input file");
			exit(EXIT_FAILURE);
		}
		if (fclose(fp2) != 0) {
			fprintf(stderr, "Error in closing input file");
			exit(EXIT_FAILURE);
		}
	} else if (strcmp(*argv, "decrypt") == 0) {
		if (--argc > 1) {
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		if (argc == 1) {
			readFromFile = 1;
			if ((fp = fopen(*argv, "r")) == NULL) {
				fprintf(stderr, "Can't open file %s for reading\n", *argv);
				exit(EXIT_FAILURE);
			}
		}
		decrypt(fp);
	} else {
		fprintf(stderr, "Unknown command, %s\n", *argv);
		usage(); /* exit */
	}
	
	/* close input file */
	if (readFromFile) {
		if (fclose(fp) != 0) {
			fprintf(stderr, "Error in closing input file");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    ./hw2 stream -p=pphrase -l=len\n");
	fprintf(stderr, "    ./hw2 encrypt -p=pphrase -out=name [pbmfile]\n");
	fprintf(stderr, "    ./hw2 merge pbmfile1 pbmfile2\n");
	fprintf(stderr, "    ./hw2 decrypt [pbmfile]\n");
	exit(EXIT_FAILURE);
}

char *parseOption(char *s)
{
	char *p = strchr(s, '=');
	if (p == NULL) return NULL; /* '=' not found */
	*p++ = '\0'; /* replace '=' with '\0' and set p point to the next char */
	return p;
}

int parseInt(const char *s)
{
	int res = 0;
	char c;
	while ((c = *s++) != '\0') {
		if (c - '0' < 0 || c - '0' > 9) return -1; /* c is not 0-9 */
		res = res * 10 + c - '0';
	}
	return res;
}

