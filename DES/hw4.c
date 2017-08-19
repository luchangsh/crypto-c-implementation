#include "hw4.h"

int main(int argc, char *argv[])
{
	FILE *ifp = stdin; /* input file pointer, default: stdin */
	FILE *tfp = NULL; /* tablefile pointer */
	int readFromFile = 0; /* 0: read from stdin; 1: read from a input file */

	argc--; argv++; /* skip argv[0] */
	if (argc <= 0) { /* only one argument */ 
		fprintf(stderr, "Command not specified\n");
		usage(); /* exit */
	}
	
	if (strcmp(*argv, "tablecheck") == 0) {
		if (--argc != 1) { /* can only have one argument after command name */
			fprintf(stderr, "Malformed command: Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		
		argv++; /* skip argv[1] */
		
		char *tablefile = NULL;
		
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* not a commandline option */
				fprintf(stderr, "Option \"%s\" doesn't start with a \"-\".\n", *argv);
				usage(); /* exit */
			}
			
			char *val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
			
			if (val == NULL) {
				fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
				usage(); /* exit */
			}
			if (*val == '\0') {
				fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
				usage(); /* exit */
			}
			
			if (strcmp(*argv, "-t") == 0) {
				tablefile = malloc(strlen(val) + 1);
				strncpy(tablefile, val, strlen(val) + 1);
			} else {
				fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
				usage(); /* exit */
			}
		}
		
		if (tablefile == NULL) { /* not specify the tablefile trough commandline option */
			fprintf(stderr, "Did not specify a tablefile\n");
			usage(); /* exit */
		}
		
		if ((tfp = fopen(tablefile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open tablefile \"%s\" for reading\n", tablefile);
			exit(EXIT_FAILURE);
		}
		
		tablecheck(tfp);
		
		free(tablefile);
		
	} else if (strcmp(*argv, "encrypt") == 0) {
		if (--argc < 2 || argc > 3) { /* can only have 2 or 3 arguments after the command */
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		
		argv++; /* skip argv[1] */
		
		char *key = NULL;
		char *tablefile = NULL;
		
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* optional input file */
				if ((ifp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
					fprintf(stderr, "Can't open input file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
				readFromFile = 1;
			} else { /* commandline options */
				char *val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
				
				if (val == NULL) {
					fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
					usage(); /* exit */
				}
				
				if (strcmp(*argv, "-k") == 0) {
					key = malloc(strlen(val) + 1);
					strncpy(key, val, strlen(val) + 1);
				} else if (strcmp(*argv, "-t") == 0) {
					tablefile = malloc(strlen(val) + 1);
					strncpy(tablefile, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
					usage(); /* exit */
				}
			}
		}
		
		if (key == NULL) { /* not specify the key trough commandline option */
			fprintf(stderr, "Did not specify a key\n");
			usage(); /* exit */
		}
		if (tablefile == NULL) { /* not specify the tablefile trough commandline option */
			fprintf(stderr, "Did not specify a tablefile\n");
			usage(); /* exit */
		}
		
		if ((tfp = fopen(tablefile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open tablefile \"%s\" for reading\n", tablefile);
			exit(EXIT_FAILURE);
		}
		
		encrypt(key, tfp, ifp);
		
		free(key);
		free(tablefile);
		
	} else if (strcmp(*argv, "decrypt") == 0) {
		if (--argc < 2 || argc > 3) { /* can only have 2 or 3 arguments after the command */
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		
		argv++; /* skip argv[1] */
		
		char *key = NULL;
		char *tablefile = NULL;
		
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* optional input file */
				if ((ifp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
					fprintf(stderr, "Can't open input file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
				readFromFile = 1;
			} else { /* commandline options */
				char *val = parseOption(*argv);
				
				if (val == NULL) { /* after parsing, *argv = key string, *val = value string */
					fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
					usage(); /* exit */
				}
				
				if (strcmp(*argv, "-k") == 0) {
					key = malloc(strlen(val) + 1);
					strncpy(key, val, strlen(val) + 1);
				} else if (strcmp(*argv, "-t") == 0) {
					tablefile = malloc(strlen(val) + 1);
					strncpy(tablefile, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
					usage(); /* exit */
				}
			}
		}
		
		if (key == NULL) { /* not specify the key trough commandline option */
			fprintf(stderr, "Did not specify a key\n");
			usage(); /* exit */
		}
		if (tablefile == NULL) { /* not specify the tablefile trough commandline option */
			fprintf(stderr, "Did not specify a tablefile\n");
			usage(); /* exit */
		}
		
		if ((tfp = fopen(tablefile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open tablefile \"%s\" for reading\n", tablefile);
			exit(EXIT_FAILURE);
		}
		
		decrypt(key, tfp, ifp);
		
		free(key);
		free(tablefile);
		
	} else if (strcmp(*argv, "encrypt3") == 0) {
		if (--argc < 2 || argc > 3)	{ /* can only have 2 or 3 arguments after the command */
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		
		argv++; /* skip argv[1] */
		
		char *key3 = NULL;
		char *tablefile = NULL;
		
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* optional input file */
				if ((ifp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
					fprintf(stderr, "Can't open input file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
				readFromFile = 1;
			} else { /* commandline options */
				char *val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
				
				if (val == NULL) {
					fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
					usage(); /* exit */
				}
				
				if (strcmp(*argv, "-k") == 0) {
					key3 = malloc(strlen(val) + 1);
					strncpy(key3, val, strlen(val) + 1);
				} else if (strcmp(*argv, "-t") == 0) {
					tablefile = malloc(strlen(val) + 1);
					strncpy(tablefile, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
					usage(); /* exit */
				}		
			}
		}
		
		if (key3 == NULL) { /* not specify the key trough commandline option */
			fprintf(stderr, "Did not specify a key\n");
			usage(); /* exit */
		}
		if (tablefile == NULL) { /* not specify the tablefile trough commandline option */
			fprintf(stderr, "Did not specify a tablefile\n");
			usage(); /* exit */
		}
		
		if ((tfp = fopen(tablefile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open tablefile \"%s\" for reading\n", tablefile);
			exit(EXIT_FAILURE);
		}
		
		encrypt3(key3, tfp, ifp);
		
		free(key3);
		free(tablefile);
		
	} else if (strcmp(*argv, "decrypt3") == 0) {
		if (--argc < 2 || argc > 3)	{ /* can only have 2 or 3 arguments after the command */
			fprintf(stderr, "Syntex error for \"%s\" command\n", *argv);
			usage(); /* exit */
		}
		
		argv++; /* skip argv[1] */
		
		char *key3 = NULL;
		char *tablefile = NULL;
		
		for (; argc > 0; argc--, argv++) {
			if (*argv[0] != '-') { /* optional input file */
				if ((ifp = fopen(*argv, "r")) == NULL) { /* fail to open input file */
					fprintf(stderr, "Can't open input file %s for reading.\n", *argv);
					exit(EXIT_FAILURE);
				}
				readFromFile = 1;
			} else { /* commandline options */
				char *val = parseOption(*argv); /* after parsing, *argv = key string, *val = value string */
				
				if (val == NULL) {
					fprintf(stderr, "Option \"%s\" contains no \"=\".\n", *argv);
					usage(); /* exit */
				}
				if (*val == '\0') {
					fprintf(stderr, "Option \"%s\" has no value.\n", *argv);
					usage(); /* exit */
				}
				
				if (strcmp(*argv, "-k") == 0) {
					key3 = malloc(strlen(val) + 1);
					strncpy(key3, val, strlen(val) + 1);
				} else if (strcmp(*argv, "-t") == 0) {
					tablefile = malloc(strlen(val) + 1);
					strncpy(tablefile, val, strlen(val) + 1);
				} else {
					fprintf(stderr, "Option \"%s\" is unknown.\n", *argv);
					usage(); /* exit */
				}		
			}
		}
		
		if (key3 == NULL) { /* not specify the key trough commandline option */
			fprintf(stderr, "Did not specify a key\n");
			usage(); /* exit */
		}
		if (tablefile == NULL) { /* not specify the tablefile trough commandline option */
			fprintf(stderr, "Did not specify a tablefile\n");
			usage(); /* exit */
		}
		
		if ((tfp = fopen(tablefile, "r")) == NULL) { /* fail to open tablefile */
			fprintf(stderr, "Can't open tablefile \"%s\" for reading\n", tablefile);
			exit(EXIT_FAILURE);
		}
		
		decrypt3(key3, tfp, ifp);
		
		free(key3);
		free(tablefile);
		
	} else {
		fprintf(stderr, "Unknown command, %s\n", *argv);
		usage(); /* exit */
	}

	/* Close files */
	if (fclose(tfp) != 0) {
		fprintf(stderr, "Error in closing the tablefile\n");
		exit(EXIT_FAILURE);
	}
	if (readFromFile) {
		if (fclose(ifp) != 0) {
			fprintf(stderr, "Error in closing the input file\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    ./hw4 tablecheck -t=tablefile\n");
	fprintf(stderr, "    ./hw4 encrypt -k=key -t=tablefile [file]\n");
	fprintf(stderr, "    ./hw4 decrypt -k=key -t=tablefile [file]\n");
	fprintf(stderr, "    ./hw4 encrypt3 -k=key3 -t=tablefile [file]\n");
	fprintf(stderr, "    ./hw4 decrypt3 -k=key3 -t=tablefile [file]\n");
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

