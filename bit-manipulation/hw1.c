#include "hw1.h"
int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	
	/* handle malformed command */
	/* no command name */
	if (argc == 1) {
		fprintf(stderr, "ERROR: missing command names\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);		
	}
	/* number of arguments > 3 */
	if (argc > 3) {
		fprintf(stderr, "ERROR: too many arguments\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	} /* If we got here, argc == 2 or 3 */
	/* options appear */
	if (*argv[1] == '-' || (argc == 3 && *argv[2] == '-')) {
		fprintf(stderr, "ERROR: can't have options\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);	
	}	
	/* unknown commands */
	if (strcmp(argv[1], "hexdump") != 0 && strcmp(argv[1], "enc-base64") != 0 &&
			strcmp(argv[1], "dec-base64") != 0) {
		fprintf(stderr, "ERROR: unknown command name\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* decide input file base on argc */	
	if (argc == 2) fp = stdin; /* argc == 2, input from stdin */
	else { /* argc == 3, input from file */
		if ((fp = fopen(argv[2], "r")) == NULL)	{ /* handle file opening */
			fprintf(stderr, "ERROR: can't open file %s for reading\n", argv[2]);
			exit(EXIT_FAILURE);
		}
	}
	
	/* argv[1] == "hexdump", "enc-base64", or "dec-base64" */
	if (strcmp(argv[1], "hexdump") == 0) hexdump(fp);
	else if (strcmp(argv[1], "enc-base64") == 0) encodeBase64(fp);
	else decodeBase64(fp); /* dec-base64 */
	
	/* handle file closing */
	if (argc == 3) { /* don't close stdin */
		if (fclose(fp) != 0) {
			fprintf(stderr, "ERROR: error in closing files\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return 0;
}

void usage(char *name)
{
	fprintf(stderr, "Usage: %s {hexdump|enc-base64|dec-base64} [file]\n", name);
}
