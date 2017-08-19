#include "hw7.h"

int main(int argc, char *argv[])
{
	FILE *fp = stdin; /* input file pointer, default: stdin */
	int readFromFile = 0; /* 0: not read, 1: read */
	
	argc--; argv++; /* skip argv[0] */
	if (argc <= 0) { /* only one argument */ 
		fprintf(stderr, "hw7: command not specified.\n");
		usage(); /* exit */
	}	
	if (strcmp(*argv, "rc4") == 0) {
		if (--argc < 1 || argc > 2) { /* can only have 1 or 2 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		char *pphrase = NULL; /* passphrase */
		int len = -1; /* the number of bytes to output */
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "hw7: option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}
			val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			if (val == NULL) {
				fprintf(stderr, "hw7: option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "hw7: option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}
			if (strcmp(*argv, "-p") == 0) {
				pphrase = malloc(strlen(val) + 1);
				strncpy(pphrase, val, strlen(val) + 1);
			} else if (strcmp(*argv, "-l") == 0) {
				len = parseInt(val);
				if (len == -1) {
					fprintf(stderr, "hw7: \"%s\" is not a valid value of len.\n", val);
					usage(); /* exit */
				}
			} else {
				fprintf(stderr, "hw7: option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}
		}
		if (pphrase == NULL) {
			fprintf(stderr, "hw7: did not specify -p=pphrase.\n");
			usage(); /* exit */
		}
		rc4(pphrase, len); /* if len = 0, then -l=len option isn't specified */
		free(pphrase);
	} else if (strcmp(*argv, "x1") == 0) {
		if (--argc > 1) { /* can only have 0 or 1 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] == '-') { /* commandline option */
				fprintf(stderr, "hw7: malformed command: Cannot specify options \"%s\".\n", *argv);
				usage(); /* exit */
			}
			/* optional input file */
			if ((fp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
				fprintf(stderr, "hw7: can't open the input file \"%s\" for reading.\n", *argv);
				exit(EXIT_FAILURE);
			}
			readFromFile = 1;
		}
		x1(fp);
	} else if (strcmp(*argv, "x2") == 0) {
		if (--argc > 1) { /* can only have 0 or 1 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] == '-') { /* commandline option */
				fprintf(stderr, "hw7: malformed command: Cannot specify options \"%s\".\n", *argv);
				usage(); /* exit */
			}
			/* optional input file */
			if ((fp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
				fprintf(stderr, "hw7: can't open the input file \"%s\" for reading.\n", *argv);
				exit(EXIT_FAILURE);
			}
			readFromFile = 1;
		}
		x2(fp);
	} else if (strcmp(*argv, "x3") == 0) {
		if (--argc > 1) { /* can only have 0 or 1 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] == '-') { /* commandline option */
				fprintf(stderr, "hw7: malformed command: Cannot specify options \"%s\".\n", *argv);
				usage(); /* exit */
			}
			/* optional input file */
			if ((fp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
				fprintf(stderr, "hw7: can't open the input file \"%s\" for reading.\n", *argv);
				exit(EXIT_FAILURE);
			}
			readFromFile = 1;
		}
		x3(fp);
	} else if (strcmp(*argv, "x4") == 0) {
		if (--argc > 1) { /* can only have 0 or 1 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] == '-') { /* commandline option */
				fprintf(stderr, "hw7: malformed command: Cannot specify options \"%s\".\n", *argv);
				usage(); /* exit */
			}
			/* optional input file */
			if ((fp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
				fprintf(stderr, "hw7: can't open the input file \"%s\" for reading.\n", *argv);
				exit(EXIT_FAILURE);
			}
			readFromFile = 1;
		}
		x4(fp);
	} else if (strcmp(*argv, "x5") == 0) {
		if (--argc < 1 || argc > 2) { /* can only have 1 or 2 argument after the command name */
			fprintf(stderr, "hw7: malformed command: Syntex error for \"%s\" command.\n", *argv);
			usage(); /* exit */
		}
		argv++; /* skip argv[1] */
		int dval = -1;
		char *val = NULL;
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* optional input file */
				if ((fp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
					fprintf(stderr, "hw7: can't open the input file \"%s\" for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
				readFromFile = 1;
			} else { /* commandline options */
				val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
				if (val == NULL) {
					fprintf(stderr, "hw7: option \"%s\" contains no \"=\".\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "hw7: option \"%s\" has no value.\n", *argv);
					usage(); /* exit */
				}
				if (strcmp(*argv, "-d") == 0) {
					dval = parseInt(val);
					if (dval == -1) {
						fprintf(stderr, "hw7: \"%s\" is not a valid value of d.\n", val);
						usage(); /* exit */
					}
				} else {
					fprintf(stderr, "hw7: option \"%s\" is unknown.\n", *argv);
					usage(); /* exit */
				}
			}
		}
		if (dval == -1) {
			fprintf(stderr, "hw7: did not specify -d=dval.\n");
			usage(); /* exit */
		}
		x5(dval, fp);
	} else {
		fprintf(stderr, "hw7: unknown command, %s.\n", *argv);
		usage(); /* exit */
	}

	/* Close files */
	if (readFromFile) {
		if (fclose(fp) != 0) {
			fprintf(stderr, "hw7: error in closing the input file.\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    ./hw7 rc4 -p=pphrase [-l=len]\n");
	fprintf(stderr, "    ./hw7 x1 [file]\n");
	fprintf(stderr, "    ./hw7 x2 [file]\n");
	fprintf(stderr, "    ./hw7 x3 [file]\n");
	fprintf(stderr, "    ./hw7 x4 [file]\n");
	fprintf(stderr, "    ./hw7 x5 -d=dval [file]\n");
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

