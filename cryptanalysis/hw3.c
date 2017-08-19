#include "hw3.h"
int main(int argc, char *argv[])
{
	FILE *ifp = stdin; /* Input file pointer, default: stdin */
	FILE *kfp = NULL; /* Key file pointer */
	int readFromFile = 0; /* boolean: read from file?  */
	int openKeyFile = 0;  /* boolean: open a key file? */
	
	argc--; argv++; /* Skip argv[0] */
	if (argc <= 0) { /* Only one argument */
		fprintf(stderr, "Command not specified.\n");
		usage(); /* exit */
	}
	if (strcmp(*argv, "keygen") == 0) {
		if (--argc != 2) {
			fprintf(stderr, "Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		char *pphrase = NULL; /* passphrase */
		int period = -1;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* Not commandline options */
				fprintf(stderr, "Not an option, %s.\n", *argv);
				usage(); /* exit */
			}
			char *val = parseOption(*argv);
			if (val == NULL) {
				fprintf(stderr, "Malformed option, %s.\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Empty value of \'%s\' option.\n", *argv);
				usage(); /* exit */
			}
			if (strcmp(*argv, "-p") == 0) {
				pphrase = malloc(strlen(val) + 1);
				strncpy(pphrase, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-t") == 0) {
				period = parseInt(val);
			} else {
				fprintf(stderr, "Unknown option, %s.\n", *argv);
				usage(); /* exit */
			}
		}
		if (pphrase == NULL || period == -1) { /* Not specify required options */
			fprintf(stderr, "Malformed command.\n");
			usage(); /* exit */
		}
		keygen(pphrase, period);
		free(pphrase);
	} else if (strcmp(*argv, "crypt") == 0)	{
		if (--argc < 1 || argc > 2) {
			fprintf(stderr, "Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		char *keyfile = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* Optional input file */
				readFromFile = 1;
				if ((ifp = fopen(*argv, "r")) == NULL) { /* Fail to open input file */
					fprintf(stderr, "Can't open file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
			} else { /* Commandline options */
				char *val = parseOption(*argv);
				if (val == NULL) {
					fprintf(stderr, "Malformed option, %s.\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Empty value of \'%s\' option.\n", *argv);
					usage(); /* exit */
				}
				if (strcmp(*argv, "-k") == 0) {
					keyfile = malloc(strlen(val) + 1);
					strncpy(keyfile, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Unknown option, %s.\n", *argv);
					usage(); /* exit */
				}			
			}
		}
		if (keyfile == NULL) { /* Not specify keyfile */
			fprintf(stderr, "Malformed command.\n");
			usage(); /* exit */
		}
		openKeyFile = 1;
		if ((kfp = fopen(keyfile, "r")) == NULL) { /* Fail to open keyfile */
			fprintf(stderr, "Can't open file %s for reading.\n", keyfile);
			exit(EXIT_FAILURE);
		}
		crypt(kfp, ifp);
		free(keyfile);
	} else if (strcmp(*argv, "invkey") == 0) {
		if (--argc != 1) {
			fprintf(stderr, "Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		openKeyFile = 1;
		if ((kfp = fopen(*argv, "r")) == NULL) { /* Fail to open keyfile */
			fprintf(stderr, "Can't open file %s for reading.\n", *argv);
			exit(EXIT_FAILURE);
		}
		invkey(kfp);
	} else if (strcmp(*argv, "histo") == 0) {
		if (--argc < 2 || argc > 3) {
			fprintf(stderr, "Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		int period = -1;
		int which = -1;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* Optional input file */
				readFromFile = 1;
				if ((ifp = fopen(*argv, "r")) == NULL) { /* Fail to open input file */
					fprintf(stderr, "Can't open file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
			} else { /* Commandline options */
				char *val = parseOption(*argv);
				if (val == NULL) {
					fprintf(stderr, "Malformed option, %s.\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Empty value of \'%s\' option.\n", *argv);
					usage(); /* exit */
				}
				if (strcmp(*argv, "-t") == 0) {
					period = parseInt(val);
				} else if (strcmp(*argv, "-i") == 0) {
					which = parseInt(val);
				} else {
					fprintf(stderr, "Unknown option, %s.\n", *argv);
					usage(); /* exit */
				}			
			}
		}
		if (period == -1 || which == -1) { /* Not specify required options */
			fprintf(stderr, "Malformed command.\n");
			usage(); /* exit */
		}
		histo(period, which, ifp);
	} else if (strcmp(*argv, "solve") == 0) {
		if (--argc != 2) {
			fprintf(stderr, "Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		int max_t = -1;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* Input file */
				readFromFile = 1;
				if ((ifp = fopen(*argv, "r")) == NULL) { /* Fail to open input file */
					fprintf(stderr, "Can't open file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
			} else { /* Commandline options */
				char *val = parseOption(*argv);
				if (val == NULL) {
					fprintf(stderr, "Malformed option, %s.\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Empty value of \'%s\' option.\n", *argv);
					usage(); /* exit */
				}
				
				if (strcmp(*argv, "-l") == 0) {
					max_t = parseInt(val);
				} else {
					fprintf(stderr, "Unknown option, %s.\n", *argv);
					usage(); /* exit */
				}			
			}
		}
		if (max_t == -1 || ifp == stdin) { /* Not specify required option and input file */
			fprintf(stderr, "Malformed command.\n");
			usage(); /* exit */
		}
		solve(max_t, ifp);
	} else { /* Unknown command */
		fprintf(stderr, "Unknown command, %s.\n", *argv);
		usage(); /* exit */
	}

	/* Close opened files */
	if (readFromFile) {
		if (fclose(ifp) != 0) {
			fprintf(stderr, "Error in closing input file.");
			exit(EXIT_FAILURE);
		}
	}
	if (openKeyFile) {
		if (fclose(kfp) != 0) {
			fprintf(stderr, "Error in closing key file.");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    ./hw3 keygen -p=pphrase -t=period\n");
	fprintf(stderr, "    ./hw3 crypt -k=keyfile [file]]\n");
	fprintf(stderr, "    ./hw3 invkey keyfile\n");
	fprintf(stderr, "    ./hw3 histo -t=period -i=which [file]\n");
	fprintf(stderr, "    ./hw3 solve -l=max_t file]\n");
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

