#include "tablecheck.h"

void tablecheck(FILE *tfp)
{
	int i, j; /* indice */
	
	/* create tables */
	unsigned char IP[IP_LEN];
	for (i = 0; i < IP_LEN; i++) IP[i] = 0;
	unsigned char IP_INV[IP_LEN];
	for (i = 0; i < IP_LEN; i++) IP_INV[i] = 0;
	unsigned char E[E_LEN];
	for (i = 0; i < E_LEN; i++) E[i] = 0;
	unsigned char P[P_LEN];
	for (i = 0; i < P_LEN; i++) P[i] = 0;
	unsigned char S1[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S1[i][j] = 0;
	unsigned char S2[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S2[i][j] = 0;
	unsigned char S3[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S3[i][j] = 0;
	unsigned char S4[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S4[i][j] = 0;
	unsigned char S5[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S5[i][j] = 0;
	unsigned char S6[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S6[i][j] = 0;
	unsigned char S7[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S7[i][j] = 0;
	unsigned char S8[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S8[i][j] = 0;
	unsigned char V[V_LEN];
	for (i = 0; i < V_LEN; i++) V[i] = 0;
	unsigned char PC1[PC1_LEN];
	for (i = 0; i < PC1_LEN; i++) PC1[i] = 0;
	unsigned char PC2[PC2_LEN];
	for (i = 0; i < PC2_LEN; i++) PC2[i] = 0;
	
	buildTables(tfp, IP, IP_INV, E, P, S1, S2, S3, S4, 
				S5, S6, S7, S8, V, PC1, PC2);
	
}

void buildTables(FILE *tfp, unsigned char *IP, unsigned char *IP_INV, unsigned char *E, unsigned char *P,
				 unsigned char S1[S_ROWS][S_COLS], unsigned char S2[S_ROWS][S_COLS], unsigned char S3[S_ROWS][S_COLS],
				 unsigned char S4[S_ROWS][S_COLS], unsigned char S5[S_ROWS][S_COLS], unsigned char S6[S_ROWS][S_COLS],
				 unsigned char S7[S_ROWS][S_COLS], unsigned char S8[S_ROWS][S_COLS], unsigned char *V,
				 unsigned char *PC1, unsigned char *PC2)
{
	int i;
	
	/* A boolean array to track if a table is specified in the tablefile */
	unsigned char found[NUM_TABLES]; /* [IP, E, P, S1, S2, S3, S4, S5, S6, S7, S8, V, PC1, PC2] */
	for (i = 0; i < NUM_TABLES; i++) found[i] = 0;

	/* buffer for reading the tablefile */
	char buffer[TF_BUF_SIZE];
	for (i = 0; i < TF_BUF_SIZE; i++) buffer[i] = 0;

	char *val;
	int numberOfLine = 0;

	while (fgets(buffer, TF_BUF_SIZE, tfp) != NULL)	{
		
		numberOfLine++;
		val = parseOption(buffer); /* parse the key=value string */
		
		if (val == NULL) {
			fprintf(stderr, "Malformed tablefile, line %d contains no \"=\".\n", numberOfLine);
			exit(EXIT_FAILURE);
		}
		if (*val == '\0') {
			fprintf(stderr, "Malformed tablefile, line %d contains no value.\n", numberOfLine);
			exit(EXIT_FAILURE);
		}
		
		*(val + strlen(val) - 1) = '\0'; /* replace the ending '\n' with '\0' */
		
		/* check if the value contains only commas and digits */
		char *p;
		for (p = val; *p != '\0'; p++) {
			if (*p != ',' && (*p - '0' < 0 || *p - '0' > 9)) {
				fprintf(stderr, "Value of line %d contains characters other than digits and comma.\n", numberOfLine);
				exit(EXIT_FAILURE);
			}
		}

		/* build tables based on the key */
		if (strcmp(buffer, "IP") == 0) {
			if (found[0]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[0] = 1;
			buildIPandIP_INVTable(IP, IP_INV, buffer, val);
		} else if (strcmp(buffer, "E") == 0) {
			if (found[1]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[1] = 1;
			buildETable(E, buffer, val);
		} else if (strcmp(buffer, "P") == 0) {
			if (found[2]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[2] = 1;
			buildPTable(P, buffer, val);
		} else if (strcmp(buffer, "S1") == 0) {
			if (found[3]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[3] = 1;
			buildSTable(S1, buffer, val);
		} else if (strcmp(buffer, "S2") == 0) {
			if (found[4]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[4] = 1;
			buildSTable(S2, buffer, val);
		} else if (strcmp(buffer, "S3") == 0) {
			if (found[5]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[5] = 1;
			buildSTable(S3, buffer, val);
		} else if (strcmp(buffer, "S4") == 0) {
			if (found[6]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[6] = 1;
			buildSTable(S4, buffer, val);
		} else if (strcmp(buffer, "S5") == 0) {
			if (found[7]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[7] = 1;
			buildSTable(S5, buffer, val);
		} else if (strcmp(buffer, "S6") == 0) {
			if (found[8]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[8] = 1;
			buildSTable(S6, buffer, val);
		} else if (strcmp(buffer, "S7") == 0) {
			if (found[9]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[9] = 1;
			buildSTable(S7, buffer, val);
		} else if (strcmp(buffer, "S8") == 0) {
			if (found[10]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[10] = 1;
			buildSTable(S8, buffer, val);
		} else if (strcmp(buffer, "V") == 0) {
			if (found[11]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[11] = 1;
			buildVTable(V, buffer, val);
		} else if (strcmp(buffer, "PC1") == 0) {
			if (found[12]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[12] = 1;
			buildPC1Table(PC1, buffer, val);
		} else if (strcmp(buffer, "PC2") == 0) {
			if (found[13]) {
				fprintf(stderr, "Table %s has already been specified.\n", buffer);
				exit(EXIT_FAILURE);
			}
			found[13] = 1;
			buildPC2Table(PC2, buffer, val);
		} else {
			fprintf(stderr, "Line %d contains unknown key %s.\n", numberOfLine, buffer);
			exit(EXIT_FAILURE);
		}
				
		/* reset buffer */
		for (i = 0; i < TF_BUF_SIZE; i++) buffer[i] = 0;
	}
	
	/* check if all the 14 tables are specified */
	if (numberOfLine != NUM_TABLES) {
		fprintf(stderr, "Malformed tablefile, tablefile contains wrong number of lines, %d.\n", numberOfLine);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_TABLES; i++) {
		if (found[i] == 0) {
			fprintf(stderr, "Malformed tablefile, some tables are not specified in the tablefile.\n");
			exit(EXIT_FAILURE);
		}
	}
}

void buildIPandIP_INVTable(unsigned char *IP, unsigned char *IP_INV, char *buffer, char *val)
{
	int index = 0; /* index of IP array */
	char *token = strtok(val, ",");
	int num = 0;
	
	while (token != NULL) {
		if (index >= IP_LEN) {
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < IP_MIN || num > IP_MAX) {
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		IP[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < IP_LEN)	{
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, IP_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[IP_MAX + 1];
	int i;
	for (i = 0; i < IP_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < IP_LEN; i++) counts[IP[i]]++;
	
	/* check if each number occurs only one time */
	for (i = IP_MIN; i <= IP_MAX; i++) {
		if (counts[i] > 1)	{
			fprintf(stderr, "(%3s) the value %d in the %s table occur too many times\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		} else if (counts[i] < 1) {
			fprintf(stderr, "(%3s) the value %d in the %s table did not occur\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		}
	}

	/* build the inverse table of IP */
	for (i = 0; i < IP_LEN; i++) IP_INV[IP[i] - 1] = i + 1;
}

void buildETable(unsigned char *E, char *buffer, char *val)
{
	int index = 0; /* index of E array */
	char *token = strtok(val, ",");
	int num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= E_LEN)	{
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < E_MIN || num > E_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		E[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < E_LEN) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, E_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[E_MAX + 1];
	int i;
	for (i = 0; i < E_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < E_LEN; i++) counts[E[i]]++;
	
	/* check if each 32 numbers occur at least one and at most two times */
	for (i = E_MIN; i <= E_MAX; i++) {
		if (counts[i] > 2)	{
			fprintf(stderr, "(%3s) the value %d in the %s table occur too many times\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		} else if (counts[i] < 1) {
			fprintf(stderr, "(%3s) the value %d in the %s table did not occur\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		}
	}
}

void buildPTable(unsigned char *P, char *buffer, char *val)
{
	int index = 0; /* index of P array */
	char *token = strtok(val, ",");
	int num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= P_LEN)	{
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < P_MIN || num > P_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		P[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < P_LEN) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, P_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[P_MAX + 1];
	int i;
	for (i = 0; i < P_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < P_LEN; i++) counts[P[i]]++;
	
	/* check if each number occurs only one time */
	for (i = P_MIN; i <= P_MAX; i++) {
		if (counts[i] > 1) {
			fprintf(stderr, "(%3s) the value %d in the %s table occur too many times\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		} else if (counts[i] < 1) {
			fprintf(stderr, "(%3s) the value %d in the %s table did not occur\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		}
	}
}

void buildSTable(unsigned char S[S_ROWS][S_COLS], char *buffer, char *val)
{
	int row = 0, col = 0; /* index of S matrix */
	int index = 0;
	char *token = strtok(val, ",");
	int num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= S_SIZE) {
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		num = parseInt(token);
		
		if (num < S_MIN || num > S_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		S[index / S_COLS][index % S_COLS] = num;
		index++;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < S_SIZE) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, S_SIZE);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number for each row */
	unsigned char counts[S_MAX + 1];
	int i;
	for (i = 0; i < S_MAX + 1; i++)	counts[i] = 0; /* initialize to 0 */
	
	for (row = 0; row < S_ROWS; row++) {
		/* count this row */
		for (col = 0; col < S_COLS; col++) counts[S[row][col]]++;
		
		/* check all the value 0-15 occur only once */
		for (i = S_MIN; i <= S_MAX; i++) {
			if (counts[i] > 1)	{
				fprintf(stderr, "(%3s) the value %d in the %s table at the row %d occur too many times\n", buffer, i, buffer, row);
				exit(EXIT_FAILURE);
			} else if (counts[i] < 1) {
				fprintf(stderr, "(%3s) the value %d in the %s table at the row %d did not occur\n", buffer, i, buffer, row);
				exit(EXIT_FAILURE);
			}
		}
		
		/* reset counts */
		for (i = 0; i < S_MAX + 1; i++)	counts[i] = 0;
	}
}

void buildVTable(unsigned char *V, char *buffer, char *val)
{
	int index = 0; /* index of V array */
	char *token = strtok(val, ",");
	int num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= V_LEN)	{
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < V_MIN || num > V_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		V[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < V_LEN) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, V_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[V_MAX + 1];
	int i;
	for (i = 0; i < V_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < V_LEN; i++) counts[V[i]]++;
	
	/* check if there are four ones and twelve twos */
	if (counts[V_MIN] != V_NUM_OF_ONES)	{
		fprintf(stderr, "(%3s) the value %d in the %s table does not occur %d times\n", buffer, V_MIN, buffer, V_NUM_OF_ONES);
		exit(EXIT_FAILURE);
	}
	if (counts[V_MAX] != V_NUM_OF_TWOS)	{
		fprintf(stderr, "(%3s) the value %d in the %s table does not occur %d times\n", buffer, V_MAX, buffer, V_NUM_OF_TWOS);
		exit(EXIT_FAILURE);
	}
}

void buildPC1Table(unsigned char *PC1, char *buffer, char *val)
{
	int index = 0; /* index of PC1 array */
	char *token = strtok(val, ",");
	int num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= PC1_LEN) {
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < PC1_MIN || num > PC1_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		PC1[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < PC1_LEN) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, PC1_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[PC1_MAX + 1];
	int i;
	for (i = 0; i < PC1_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < PC1_LEN; i++) counts[PC1[i]]++;
	
	/* check if each number occurs only one time */
	for (i = PC1_MIN; i <= PC1_MAX; i++) {
		if (i % 8 != 0) {
			if (counts[i] > 1)	{
				fprintf(stderr, "(%3s) the value %d in the %s table occur too many times\n", buffer, i, buffer);
				exit(EXIT_FAILURE);
			} else if (counts[i] < 1) {
				fprintf(stderr, "(%3s) the value %d in the %s table did not occur\n", buffer, i, buffer);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void buildPC2Table(unsigned char *PC2, char *buffer, char *val)
{
	int index = 0; /* index of PC2 array */
	char *token = strtok(val, ",");
	unsigned char num = 0;
	
	/* tokenize the value */
	while (token != NULL) {
		if (index >= PC2_LEN) {
			fprintf(stderr, "(%3s) too many values read\n", buffer);
			exit(EXIT_FAILURE);
		}
		
		num = parseInt(token);
		
		if (num < PC2_MIN || num > PC2_MAX)	{
			fprintf(stderr, "(%3s) the value %d in the %s table is out of range\n", buffer, num, buffer);
			exit(EXIT_FAILURE);
		}
		
		PC2[index++] = num;
		token = strtok(NULL, ",");
	}
	
	/* check if enough number of value */
	if (index < PC2_LEN) {
		fprintf(stderr, "(%3s) %d values read while %d was expected\n", buffer, index, PC2_LEN);
		exit(EXIT_FAILURE);
	}
	
	/* count the frequency of each number */
	unsigned char counts[PC2_MAX + 1];
	int i;
	for (i = 0; i < PC2_MAX + 1; i++) counts[i] = 0; /* initialize to 0 */
	for (i = 0; i < PC2_LEN; i++) counts[PC2[i]]++;
	
	/* check if each number occurs only one time */
	int numOfCountZero = 0;
	for (i = PC2_MIN; i <= PC2_MAX; i++) {
		if (counts[i] > 1)	{
			fprintf(stderr, "(%3s) the value %d in the %s table occur too many times\n", buffer, i, buffer);
			exit(EXIT_FAILURE);
		} else if (counts[i] == 0) {
			if (numOfCountZero >= 8) {
				fprintf(stderr, "(%3s) there are more than 8 values in the %s table did not occur\n", buffer, buffer);
				exit(EXIT_FAILURE);
			}
			numOfCountZero++;
		}
	}
}
