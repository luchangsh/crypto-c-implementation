#include "hw6.h"

int main(int argc, char *argv[])
{
	FILE *pfp = NULL; /* primesfile pointer */
	FILE *rfp = NULL; /* rndfile pointer */
	int openPrimesFile = 0; /* 0: not open, 1: open */
	int openRndFile = 0; /* 0: not open, 1: open */
	
	argc--; argv++; /* skip argv[0] */
	if (argc <= 0) { /* only one argument */ 
		fprintf(stderr, "Command not specified.\n");
		usage(); /* exit */
	}
	
	if (strcmp(*argv, "primes") == 0) {
		if (--argc != 1) { /* can only have one argument after the command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
	
		int maxval = -1;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}

			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}

			if (strcmp(*argv, "-n") == 0) {
				maxval = parseInt(val);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}			
		}

		if (maxval == -1) {
			fprintf(stderr, "Malformed maxval.\n");
			usage(); /* exit */
		}

		primes(maxval);

	} else if (strcmp(*argv, "trialdiv") == 0) {
		if (--argc != 2) { /* can only have two arguments after the command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */

		char *number = NULL;
		char *primesfile = NULL;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}

			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}

			if (strcmp(*argv, "-n") == 0) {
				number = malloc(strlen(val) + 1);
				strncpy(number, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-p") == 0) {
				primesfile = malloc(strlen(val) + 1);
				strncpy(primesfile, val, strlen(val) + 1);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}			
		}

		if (number == NULL) {
			fprintf(stderr, "Did not specify -n=number.\n");
			usage(); /* exit */
		}
		if (primesfile == NULL) {
			fprintf(stderr, "Did not specify -p=primesfile.\n");
			usage(); /* exit */
		}

		if ((pfp = fopen(primesfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open primesfile \"%s\" for reading.\n", primesfile);
			exit(EXIT_FAILURE);
		}
		openPrimesFile = 1;

		trialdiv(number, pfp);

		free(number);
		free(primesfile);

	} else if (strcmp(*argv, "millerrabin") == 0) {
		if (--argc != 3) { /* can only have three arguments after the command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */

		char *number = NULL;
		int maxitr = -1;
		char *primesfile = NULL;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}

			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}

			if (strcmp(*argv, "-n") == 0) {
				number = malloc(strlen(val) + 1);
				strncpy(number, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-t") == 0) {
				maxitr = parseInt(val);
			} else if (strcmp(*argv, "-p") == 0) {
				primesfile = malloc(strlen(val) + 1);
				strncpy(primesfile, val, strlen(val) + 1);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}			
		}

		if (number == NULL) {
			fprintf(stderr, "Did not specify -n=number.\n");
			usage(); /* exit */
		}
		if (maxitr == -1) {
			fprintf(stderr, "Malformed maxitr.\n");
			usage(); /* exit */
		}
		if (primesfile == NULL) {
			fprintf(stderr, "Did not specify -p=primesfile.\n");
			usage(); /* exit */
		}

		if ((pfp = fopen(primesfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open primesfile \"%s\" for reading.\n", primesfile);
			exit(EXIT_FAILURE);
		}
		openPrimesFile = 1;

		millerrabin(number, maxitr, pfp);

		free(number);
		free(primesfile);

	} else if (strcmp(*argv, "rndsearch") == 0) {
		if (--argc != 4) { /* can only have four arguments after the command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */

		int numbits = -1;
		int maxitr = -1;
		char *primesfile = NULL;
		char *rndfile = NULL;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}

			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}

			if (strcmp(*argv, "-k") == 0) {
				numbits = parseInt(val);
			} else if (strcmp(*argv, "-t") == 0) {
				maxitr = parseInt(val);
			} else if (strcmp(*argv, "-p") == 0) {
				primesfile = malloc(strlen(val) + 1);
				strncpy(primesfile, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-r") == 0) {
				rndfile = malloc(strlen(val) + 1);
				strncpy(rndfile, val, strlen(val) + 1);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}			
		}

		if (numbits == -1) {
			fprintf(stderr, "Malformed numbits.\n");
			usage(); /* exit */
		}
		if (maxitr == -1) {
			fprintf(stderr, "Malformed maxitr.\n");
			usage(); /* exit */
		}
		if (primesfile == NULL) {
			fprintf(stderr, "Did not specify -p=primesfile.\n");
			usage(); /* exit */
		}
		if (rndfile == NULL) {
			fprintf(stderr, "Did not specify -r=rndfile.\n");
			usage(); /* exit */
		}

		if ((pfp = fopen(primesfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open primesfile \"%s\" for reading.\n", primesfile);
			exit(EXIT_FAILURE);
		}
		if ((rfp = fopen(rndfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open rndfile \"%s\" for reading.\n", rndfile);
			exit(EXIT_FAILURE);
		}
		openPrimesFile = 1;
		openRndFile = 1;

		rndsearch(numbits, maxitr, pfp, rfp);

		free(primesfile);
		free(rndfile);

	} else if (strcmp(*argv, "maurer") == 0) {
		if (--argc != 3) { /* can only have three arguments after the command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */

		int numbits = -1;
		char *primesfile = NULL;
		char *rndfile = NULL;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}

			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}

			if (strcmp(*argv, "-k") == 0) {
				numbits = parseInt(val);
			} else if (strcmp(*argv, "-p") == 0) {
				primesfile = malloc(strlen(val) + 1);
				strncpy(primesfile, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-r") == 0) {
				rndfile = malloc(strlen(val) + 1);
				strncpy(rndfile, val, strlen(val) + 1);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}			
		}

		if (numbits == -1) {
			fprintf(stderr, "Malformed numbits.\n");
			usage(); /* exit */
		}
		if (primesfile == NULL) {
			fprintf(stderr, "Did not specify -p=primesfile.\n");
			usage(); /* exit */
		}
		if (rndfile == NULL) {
			fprintf(stderr, "Did not specify -r=rndfile.\n");
			usage(); /* exit */
		}

		if ((pfp = fopen(primesfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open primesfile \"%s\" for reading.\n", primesfile);
			exit(EXIT_FAILURE);
		}
		if ((rfp = fopen(rndfile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open rndfile \"%s\" for reading.\n", rndfile);
			exit(EXIT_FAILURE);
		}
		openPrimesFile = 1;
		openRndFile = 1;

		maurer(numbits, pfp, rfp);

		free(primesfile);
		free(rndfile);

	} else {
		fprintf(stderr, "Unknown command, %s\n", *argv);
		usage(); /* exit */
	}

	/* Close files */
	if (openPrimesFile) {
		if (fclose(pfp) != 0) {
			fprintf(stderr, "Error in closing the primefile\n");
			exit(EXIT_FAILURE);
		}
	}
	if (openRndFile) {
		if (fclose(rfp) != 0) {
			fprintf(stderr, "Error in closing the rndfile\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    ./hw6 primes -n=maxval\n");
	fprintf(stderr, "    ./hw6 trialdiv -n=number -p=primesfile\n");
	fprintf(stderr, "    ./hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
	fprintf(stderr, "    ./hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
	fprintf(stderr, "    ./hw6 maurer -k=numbits -p=primesfile -r=rndfile\n");
	exit(EXIT_FAILURE);
}

char *parseOption(char *s)
{
	char *p = strchr(s, '=');
	if (p == NULL) { /* '=' not found */
		return NULL;
	}
	*p++ = '\0'; /* replace '=' with '\0' and set p point to the next char */
	return p;
}

int parseInt(char *s)
{
	int res = 0;
	char c;
	while ((c = *s++) != '\0') {
		if (c - '0' < 0 || c - '0' > 9) return -1; /* c is not 0-9 */
		res = res * 10 + c - '0';
	}
	return res;
}

