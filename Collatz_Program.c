/*  Collatz conjecture program (y=3x+1) by Fumihide Shiraishi at Kyushu University in Japan 
/* 1. Features
 *   (1) Depending on the memory capacity of the computer used, the program can 
 *       compute sequences for initial values up to approximately 100 digits.
 *   (2) Each iterated value is displayed on the screen either right‑aligned or
 *       left‑aligned. The computed values can also be saved.
 *   (3) All computed values are simultaneously written to the hard disk
 *       (output file name: output.dat).
 *   (4) Each computed value is labeled as either odd or even.
 *   (5) Each computed value is also labeled with the type of the next Collatz
 *       operation. The following numbers are assigned:
 *       For odd values:
 *         A‑operations:  A → A ...1,  A → A' ...2,  A → B ...3,  A → C ...4
 *         B‑operations:  B → A ...5,  B → A' ...6,  B → B ...7
 *       For even values:
 *         C‑operations:  C → A ...8,  C → A' ...9,  C → B ...10, C → C ...11
 *   (6) At the end of the output, the program displays the total counts and
 *       percentages of each Collatz operation type, as well as the counts and
 *       percentages of operation types for each computed value.
 * 2. What to do when running the program
 *   (1) At line 43, set the print format for displaying computed values:
 *         Print format: 1 = right‑aligned; 2 = left‑aligned
 *   (2) Enter the initial value when the program starts.
 *   (3) After the computation finishes, move the output file (output.dat) to
 *       another location if you wish to save the results.
 * 3. Notes
 *   (1) This program is designed specifically for the function y = 3x + 1.
 *       However, by selecting the corresponding formulas at lines 161–164,
 *       you can also compute sequences for y = x + 1, y = 5x + 1, and y = 7x + 1.
 *       Note that the displayed Collatz operation types correspond only to
 *       y = 3x + 1 and should therefore be ignored for the other formulas.
 *   (2) The multi‑precision arithmetic routines were adapted from the program
 *       in the book: Asao Kasai,“Introduction to Algorithms with the C Programming 
 *       Language (5th Edition)”, Gijutsu‑Hyoron Co., 2023.
 */


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define print_form 1        // Print format: 1... right‑aligned; 2... left‑aligned

void long_add(short a[], short b[], short c[], short);
void long_mul(short a[], short, short c[], short);
void long_div(short a[], short k, short c[], short N, short* rem);
void print(short c[], short, int, int);
void ins(short a[], short c[], short);
void print2(short c[], short,int, int);

void odd_number(short a[], short b[], short c[], short N, int* type);
void even_number(short a[], short b[], short c[], short N, int* type);
void str_extract(char s[], int start, int char_len, char ss[]);
void owari1(int A1, int A2, int A3, int A4, int B1, int B2, int B3,int C1, int C2, int C3, int C4);

FILE* fpw;

int main(void)
{
	short slen_4,sdif, N, NN, Ninc=30;
	short* a, * b, * c;
	short rem=0;
	char s[120], ss[5];
	int i, j, QQ = 5000;

	for (i = 0; i < 120; i++) s[i] = '\0';
	for (i = 0; i < 5; i++) ss[i] = '\0';

	printf("\nInput an initial value (natural number) =　");
	scanf_s("%s",s, 120); 
	size_t slen = strlen(s); 
	N = ((slen - 1) / 4 + 1); NN = N+Ninc; 

	a = (short*)malloc(sizeof(short) * (NN + Ninc + 1));
	b = (short*)malloc(sizeof(short) * (NN + Ninc + 1));
	c = (short*)malloc(sizeof(short) * (NN + Ninc + 1));

	int type=0, A1, A2, A3, A4, A_total;
	int B1, B2, B3, B_total;
	int C1, C2, C3, C4, C_total;
	int Total = 0, owari=0;
	float p_A1, p_A2, p_A3, p_A4, p_A_total;
	float p_B1, p_B2, p_B3, p_B_total;
	float p_C1, p_C2, p_C3, p_C4, p_C_total;

	A1 = 0;      
	A2 = A3 = A4 = A_total = 0;
	B1 = B2 = B3 = B_total = 0;
	C1 = C2 = C3 = C4 = C_total = 0;

	int k,flag;

	if ((fpw = fopen("output.dat", "w")) == NULL) {         //** Open of output file
		printf("Output file cannot be opened.\n");
		exit(1);
	}

	for (i = 0; i < NN; i++) a[i] = b[i] = c[i] = 0;

	if (slen % 4 != 0)                   // Calculate the number of digits corresponding to the number of 4‑digit blocks in the input value
		slen_4 = slen + (4 - slen % 4);  // For example, an input of 11 becomes 4 digits, and 12345 becomes 8 digits.
	else
		slen_4 = slen; 

	sdif = slen_4 - slen;

	for (i = 0; i < NN; i++) {
		for (j = 0; j < 4; j++) ss[j] = '\0';

		if (slen <= 4)	
			strcpy(ss, s);
		else
			if(i==0)
				str_extract(s, 0,4-sdif, ss);
			else
				str_extract(s, 4-sdif+4*(i-1), 4, ss);

		a[i+Ninc] = atoi(ss);
	}

	ins(a, c, NN);   

	j = -1;
	while (1) {
		printf("\n"); 
		fprintf(fpw, "\n");
		if (a[NN-1] % 2 == 1) {       // Odd number: determine whether it is odd or even using only the last array element.
			type = 0;

			fflush(fpw);

			odd_number(a, b, c, NN, &type);        

			switch (type) {
			case 1: A1 = A1 + 1;
				break;
			case 2: A2 = A2 + 1;
				break;
			case 3: A3 = A3 + 1;
				break;
			case 4: A4 = A4 + 1;
				break;
			case 5: B1 = B1 + 1;
				break;
			case 6: B2 = B2 + 1;
				break;
			case 7: B3 = B3 + 1;
				break;
			}

			j = j + 1; if (j > QQ) break;
			printf("#%d Odd   ",j);
			fprintf(fpw, "#%d Odd  ", j);
			if (print_form == 1)
				print(a, NN, type, owari);
			else
				print2(a, NN, type, owari);

			//******************************************//
			long_mul(a, 3, c, NN);     //y=3x+1
//			long_mul(a, 1, c, NN);     //y=x+1
//			long_mul(a, 5, c, NN);     //y=5x+1
//			long_mul(a, 7, c, NN);     //y=7x+1
			//******************************************//

			ins(c, a, NN); 
			b[NN - 1] = 1; 
			long_add(a, b, c, NN);  
			ins(c, a, NN); 
		}
		else 
		{                           
			while (1) {
				type = 0;

				even_number(a, b, c, NN, &type);  

				switch (type) {
				case 1: C1 = C1 + 1;
					break;
				case 2: C2 = C2 + 1;
					break;
				case 3: C3 = C3 + 1;
					break;
				case 4: C4 = C4 + 1;
					break;
				}

				j = j + 1;
				printf("#%d \x1b[32mEven  ",j);
				fprintf(fpw, "#%d Even  ", j);
				printf("\x1b[37m");

				if (print_form == 1)
					print(c, NN, type+7, owari);
				else
					print2(c, NN, type+7, owari);

				long_div(a, 2, c, NN, &rem);

				if (c[NN - 1] % 2 == 1) {     // Even number: determine whether it is odd or even using only the last array element.
					ins(c, a, NN);
					break;
				}		          
				ins(c, a, NN); printf("\n"); fprintf(fpw, "\n");
			}
		}

		flag = 0;
		for (k = 0; k < NN-1; k++)
			if (c[k]!= 0) flag = 1;
		if (flag == 0 && c[NN-1] == 1) break;
	}

	printf("\n#%d \x1b[32mEven  ", j+1);
	fprintf(fpw, "\n#%d Even  ", j+1);
	printf("\x1b[37m");

	owari = 1;
	if (print_form == 1)
		print(c, NN, type, owari);
	else
		print2(c, NN, type, owari);
	printf("\n");
	fprintf(fpw, "\n");
	if (j < QQ) {
		printf("=========  Converged！ =========   \n\n");
		fprintf(fpw, "=========  Converged！ =========   \n\n");
	}
	else {
		printf("=========  Not converged！ =========   \n\n");
		fprintf(fpw, "=========  Not converged！ =========   \n\n");
	}
	A1 = A1 + 1;                  
	A_total = A1 + A2 + A3 + A4;   
	B_total = B1 + B2 + B3;
	C_total = C1 + C2 + C3 + C4;
	Total = A_total + B_total + C_total;

	printf("**** The number of each Collatz operation type ****\n");
	fprintf(fpw, "**** The number of each Collatz operation type ****\n");
	printf("A1=%d\tA2=%d\tA3=%d\tA4=%d\tTotal=%d\n", A1, A2, A3, A4, A_total);
	printf("B1=%d\tB2=%d\tB3=%d\t\tTotal=%d\n", B1, B2, B3, B_total);
	printf("C1=%d\tC2=%d\tC3=%d\tC4=%d\tTotal=%d\n", C1, C2, C3, C4, C_total);

	fprintf(fpw, "A1=%d\tA2=%d\tA3=%d\tA4=%d\tTotal=%d\n", A1, A2, A3, A4, A_total);
	fprintf(fpw, "B1=%d\tB2=%d\tB3=%d\t\tTotal=%d\n", B1, B2, B3, B_total);
	fprintf(fpw, "C1=%d\tC2=%d\tC3=%d\tC4=%d\tTotal=%d\n", C1, C2, C3, C4, C_total);

	p_A1 = (float)A1 / Total;
	p_A2 = (float)A2 / Total;
	p_A3 = (float)A3 / Total;
	p_A4 = (float)A4 / Total;
	p_B1 = (float)B1 / Total;
	p_B2 = (float)B2 / Total;
	p_B3 = (float)B3 / Total;
	p_C1 = (float)C1 / Total;
	p_C2 = (float)C2 / Total;
	p_C3 = (float)C3 / Total;
	p_C4 = (float)C4 / Total;
	p_A_total = p_A1 + p_A2 + p_A3 + p_A4;
	p_B_total = p_B1 + p_B2 + p_B3;
	p_C_total = p_C1 + p_C2 + p_C3 + p_C4;

	printf("..... Percentage\n");
	fprintf(fpw, "..... Percentage\n");

	printf("A1=%5.3f  A2=%5.3f  A3=%5.3f  A4=%5.3f  Total=%5.3f\n", p_A1, p_A2, p_A3, p_A4, p_A_total);
	printf("B1=%5.3f  B2=%5.3f  B3=%5.3f        Total=%5.3f\n", p_B1, p_B2, p_B3, p_B_total);
	printf("C1=%5.3f  C2=%5.3f  C3=%5.3f  C4=%5.3f  Total=%5.3f\n", p_C1, p_C2, p_C3, p_C4, p_C_total);
	printf("----- R [=C_total /(A_total + B_total-1)]=%10.5f -----\n", ((float)C_total / (float)(A_total + B_total-1)));
	            // Compute this by subtracting 1 from the number of occurrences of odd denominators.

	fprintf(fpw, "A1=%5.3f\tA2=%5.3f\tA3=%5.3f\tA4=%5.3f\tTotal=%5.3f\n", p_A1, p_A2, p_A3, p_A4, p_A_total);
	fprintf(fpw, "B1=%5.3f\tB2=%5.3f\tB3=%5.3f\t\tTotal=%5.3f\n", p_B1, p_B2, p_B3, p_B_total);
	fprintf(fpw, "C1=%5.3f\tC2=%5.3f\tC3=%5.3f\tC4=%5.3f\tTotal=%5.3f\n", p_C1, p_C2, p_C3, p_C4, p_C_total);
	fprintf(fpw, "----- R [=C_total /(A_total + B_total-1)]=%10.5f -----\n", ((float)C_total / (float)(A_total + B_total-1)));

	fflush(fpw);

	free(a);
	free(b);
	free(c);
	fclose(fpw);

	return 0;
}

void odd_number(short a[], short b[], short c[], short NN, int* type)
{
	short rem;
	int i; 
	short a1[120], b1[120], c1[120];

	for (i = 0; i < NN; i++) {
		a1[i] = 0; b1[i] = 0; c1[i] = 0;
	}

//  A operation ---------------------
	//if ((x + 31) % 32 == 0) *type = 1; 
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 31;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 32, c1, NN, &rem);
	if (rem == 0) *type = 1; 

	//if ((x + 15) % 32 == 0) *type = 2;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 15;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 32, c1, NN, &rem);
	if (rem == 0) *type = 2;

	//if ((x + 7) % 16 == 0) *type = 3;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 7;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 16, c1, NN, &rem);
	if (rem == 0) *type = 3;  

	//if ((x + 3) % 8 == 0) *type = 4;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 3;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 8, c1, NN, &rem);
	if (rem == 0) *type = 4;

//　B operation ------------------
	//if ((x + 5) % 16 == 0) *type = 5;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 5;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 16, c1, NN, &rem);
	if (rem == 0) *type = 5;

	//if ((x + 13) % 16 == 0) *type = 6;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 13;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 16, c1, NN, &rem);
	if (rem == 0) *type = 6;

	//if ((x + 1) % 8 == 0) *type = 7;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 1;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 8, c1, NN, &rem);
	if (rem == 0) *type = 7;
}

void even_number(short a[], short b[], short c[], short NN, int* type)
{
	short rem;
	int i;
	short a1[120], b1[120], c1[120];


	for (i = 0; i < NN; i++) {
		a1[i] = b1[i]=c1[i] = 0;
	}

//  C operation ------------------
//	if ((x + 14) % 16 == 0) *type = 1;              
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 14;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 16, c1, NN, &rem);
	if (rem == 0) *type = 1;

//	if ((x + 6) % 16 == 0) *type = 2;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 6;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 16, c1, NN, &rem);
	if (rem == 0) *type = 2;

//	if ((x + 2) % 8 == 0) *type = 3;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 2;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 8, c1, NN, &rem);
	if (rem == 0) *type = 3;

//	if (x % 4 == 0) *type = 4;
	for (i = 0; i < NN; i++) { a1[i] = a[i]; b1[i] = b[i]; c1[i] = c[i]; }
	b1[NN - 1] = 0;
	long_add(a1, b1, c1, NN);     //+31
	ins(c1, a1, NN);
	long_div(a1, 4, c1, NN, &rem);
	if (rem == 0) *type = 4;
}

void long_add(short a[], short b[], short c[], short NN)
{
	short i, cy = 0;

	for (i = NN - 1; i >= 0; i--) {
		c[i] = a[i] + b[i] + cy;
		if (c[i] < 10000)
			cy = 0;
		else {
			c[i] = c[i] - 10000;
			cy = 1;
		}
	}
}

void long_mul(short a[], short b, short c[], short NN)
{
	short i; long d, cy = 0;
	for (i = NN - 1; i >= 0; i--) {
		d = a[i];
		c[i] = (d * b + cy) % 10000;
		cy = (d * b + cy) / 10000;
	}
}

void long_div(short a[], short k, short c[], short NN, short *rem)
{
	short i; long d;
	*rem = 0;
	for (i = 0; i < NN; i++) {
		d = a[i];
		c[i] = (short)((d + *rem) / k);
		if (i < NN - 1)
			*rem = ((d + *rem) % k) * 10000;
		else
			*rem = (d + *rem) % k;
	}
}

void print(short c[], short NN, int type, int owari)
{
	short i;
	int flag = 0;
	char ctype[10];
	static int K = 0, kind2=0;
	static int A1 = 0, A2 = 0, A3 = 0, A4 = 0;
	static int B1 = 0, B2 = 0, B3 = 0;
	static int C1 = 0, C2 = 0, C3 = 0, C4 = 0, itime=-1;
	int kind = 0;

	K = K + 1;   
	itime = itime + 1;

	if (type > 0 && type <= 4) kind=1;
	if (type > 4 && type <= 7) kind=2;
	if (type > 7 && type <= 11) kind=3;

	if (K==1) kind2 = kind; 

	for (i = 0; i < 10; i++)ctype[i] = '\0';

	switch (type) {
	case 1: strcpy_s(ctype,sizeof(ctype), "A->A");
		if (K == 1) A1 = A1 + 1;
		break;
	case 2: strcpy_s(ctype, sizeof(ctype), "A->A'");
		if (K == 1) A2 = A2 + 1;
		break;
	case 3: strcpy_s(ctype, sizeof(ctype), "A->B");
		if (K == 1) A3 = A3 + 1;
		break;
	case 4: strcpy_s(ctype, sizeof(ctype), "A'->C");
		if (K == 1) A4 = A4 + 1;
		break;
	case 5: strcpy_s(ctype, sizeof(ctype), "B->A");
		if (K == 1) B1 = B1 + 1;
		break;
	case 6: strcpy_s(ctype, sizeof(ctype), "B->A'");
		if (K == 1) B2 = B2 + 1;
		break;
	case 7: strcpy_s(ctype, sizeof(ctype),"B->B");
		if (K == 1) B3 = B3 + 1;
		break;
	case 8: strcpy_s(ctype, sizeof(ctype),"C->A");
		if (K == 1) C1 = C1 + 1;
		break;
	case 9: strcpy_s(ctype, sizeof(ctype),"C->A'");
		if (K == 1) C2 = C2 + 1;
		break;
	case 10: strcpy_s(ctype, sizeof(ctype),"C ->B");
		if (K == 1) C3 = C3 + 1;
		break;
	case 11: strcpy_s(ctype, sizeof(ctype),"C ->C");
		if (K == 1) C4 = C4 + 1;
		break;
	}

	if (owari == 1) A1 = A1 - 1;   // Subtract 1 because A1 has already been counted when it becomes 1.

	for (i = 0; i < NN; i++)
		if (c[i] != 0 || flag == 1) {
			printf("%04d ", c[i]);
			fprintf(fpw, "%04d ", c[i]);
			flag = 1;
		}

	if (K == 1) {
		printf("\tType%d\t%s ***\n", type, ctype);
		fprintf(fpw, "\tType%d\t%s ***\n", type, ctype);
	}
	else {
		printf("\tType%d\t%s\n", type, ctype);
		fprintf(fpw, "\tType%d\t%s\n", type, ctype);
	}

	if (owari == 1) owari1(A1, A2, A3, A4, B1, B2, B3, C1, C2, C3, C4);

	if (kind2 == 1 && K == 3) K = 0;
	if (kind2 == 2 && K == 2) K = 0;
	if (kind2 == 3 && K == 1) K = 0;
}

void print2(short c[], short NN, int type, int owari)
{
	short i,j=0;
	int flag = 0;
	char ctype[10];
	static int K = 0, kind2 = 0;
	static int A1 = 0, A2 = 0, A3 = 0, A4 = 0;
	static int B1 = 0, B2 = 0, B3 = 0;
	static int C1 = 0, C2 = 0, C3 = 0, C4 = 0, itime = -1;
	int kind = 0;

	K = K + 1;   
	itime = itime + 1;

	if (type > 0 && type <= 4) kind = 1;
	if (type > 4 && type <= 7) kind = 2;
	if (type > 7 && type <= 11) kind = 3;

	if (K == 1) kind2 = kind;

	for (i = 0; i < 10; i++)ctype[i] = '\0';

	switch (type) {
	case 1: strcpy_s(ctype, sizeof(ctype), "A->A");
		if (K == 1) A1 = A1 + 1;
		break;
	case 2: strcpy_s(ctype, sizeof(ctype), "A->A'");
		if (K == 1) A2 = A2 + 1;
		break;
	case 3: strcpy_s(ctype, sizeof(ctype), "A->B");
		if (K == 1) A3 = A3 + 1;
		break;
	case 4: strcpy_s(ctype, sizeof(ctype), "A'->C");
		if (K == 1) A4 = A4 + 1;
		break;
	case 5: strcpy_s(ctype, sizeof(ctype), "B->A");
		if (K == 1) B1 = B1 + 1;
		break;
	case 6: strcpy_s(ctype, sizeof(ctype), "B->A'");
		if (K == 1) B2 = B2 + 1;
		break;
	case 7: strcpy_s(ctype, sizeof(ctype), "B->B");
		if (K == 1) B3 = B3 + 1;
		break;
	case 8: strcpy_s(ctype, sizeof(ctype), "C->A");
		if (K == 1) C1 = C1 + 1;
		break;
	case 9: strcpy_s(ctype, sizeof(ctype), "C->A'");
		if (K == 1) C2 = C2 + 1;
		break;
	case 10: strcpy_s(ctype, sizeof(ctype), "C ->B");
		if (K == 1) C3 = C3 + 1;
		break;
	case 11: strcpy_s(ctype, sizeof(ctype), "C ->C");
		if (K == 1) C4 = C4 + 1;
		break;
	}

	if (owari == 1) A1 = A1 - 1;   // Subtract 1 because A1 has already been counted when it becomes 1.

	printf("\n");
	printf("%d  %d  %d  %d\n", A1, A2, A3, A4);
	printf("%d  %d  %d\n", B1, B2, B3);
	printf("%d  %d  %d  %d\n", C1, C2, C3, C4);

	fprintf(fpw, "\n");
	fprintf(fpw, "%d\t%d\t%d\t%d\n", A1, A2, A3, A4);
	fprintf(fpw, "%d\t%d\t%d\n", B1, B2, B3);
	fprintf(fpw, "%d\t%d\t%d\t%d\n", C1, C2, C3, C4);

	for (i = 0; i < NN; i++) {
		if (i == 0) {
			if (c[0] == 0) {
				printf("");
			}
			else{
				printf("%d ", c[i]);
				fprintf(fpw, "%d ", c[i]);    
				flag = 1;
			}
		}

		if (i > 0 ) {
			if (flag == 0 && c[i] == 0) {
				printf("");
				fprintf(fpw, "");
			}
			if (c[i] != 0 || flag == 1) {
				if (flag == 0)
				{
					printf("%d ", c[i]); 
					fprintf(fpw, "%d ", c[i]);
					flag = 1;
				}
				else {
					printf("%04d ", c[i]);
					fprintf(fpw, "%04d ", c[i]);
				}
			}
		}
	}

	if (K == 1) {
		printf("\tType%d\t%s ***\n", type, ctype);
		fprintf(fpw, "\tType%d\t%s ***\n", type, ctype);
	}
	else {
		printf("\tType%d\t%s\n", type, ctype);
		fprintf(fpw, "\tType%d\t%s\n", type, ctype);
	}
	fprintf(fpw, "\n");

	if (owari == 1) owari1(A1, A2, A3, A4, B1, B2, B3, C1, C2, C3, C4);

	if (kind2 == 1 && K == 3) K = 0;
	if (kind2 == 2 && K == 2) K = 0;
	if (kind2 == 3 && K == 1) K = 0;
}

void owari1(A1, A2, A3, A4, B1, B2, B3, C1, C2, C3, C4)
{
	int Total, A_total = 0, B_total = 0, C_total = 0;
	float p_A1, p_A2, p_A3, p_A4, p_A_total;
	float p_B1, p_B2, p_B3, p_B_total;
	float p_C1, p_C2, p_C3, p_C4, p_C_total;

	A_total = A1 + A2 + A3 + A4;
	B_total = B1 + B2 + B3;
	C_total = C1 + C2 + C3 + C4;
	Total = A_total + B_total + C_total;

	printf("\n**** The actual number of each Collatz operation type ****\n");
	fprintf(fpw, "\n**** The actual number of each Collatz operation type ****\n");

	printf("AA1=%d\tAA2=%d\tAA3=%d\tAA4=%d\tAA_total=%d\n", A1, A2, A3, A4, A_total);
	printf("BB1=%d\tBB2=%d\tBB3=%d\t\tBB_total=%d\n", B1, B2, B3, B_total);
	printf("CC1=%d\tCC2=%d\tCC3=%d\tCC4=%d\tCC_total=%d\n", C1, C2, C3, C4, C_total);

	fprintf(fpw, "AA1=%d\tAA2=%d\tAA3=%d\tAA4=%d\tAA_total=%d\n", A1, A2, A3, A4, A_total);
	fprintf(fpw, "BB1=%d\tBB2=%d\tBB3=%d\t\tBB_total=%d\n", B1, B2, B3, B_total);
	fprintf(fpw, "CC1=%d\tCC2=%d\tCC3=%d\tCC4=%d\tCC_total=%d\n", C1, C2, C3, C4, C_total);

	p_A1 = (float)A1 / Total;
	p_A2 = (float)A2 / Total;
	p_A3 = (float)A3 / Total;
	p_A4 = (float)A4 / Total;
	p_B1 = (float)B1 / Total;
	p_B2 = (float)B2 / Total;
	p_B3 = (float)B3 / Total;
	p_C1 = (float)C1 / Total;
	p_C2 = (float)C2 / Total;
	p_C3 = (float)C3 / Total;
	p_C4 = (float)C4 / Total;
	p_A_total = p_A1 + p_A2 + p_A3 + p_A4;
	p_B_total = p_B1 + p_B2 + p_B3;
	p_C_total = p_C1 + p_C2 + p_C3 + p_C4;

	printf("..... Percentage\n");
	fprintf(fpw, "..... Percentage\n");

	printf("AA1=%5.3f  AA2=%5.3f  AA3=%5.3f  AA4=%5.3f  Total=%5.3f\n", p_A1, p_A2, p_A3, p_A4, p_A_total);
	printf("BB1=%5.3f  BB2=%5.3f  BB3=%5.3f                Total=%5.3f\n", p_B1, p_B2, p_B3, p_B_total);
	printf("CC1=%5.3f  CC2=%5.3f  CC3=%5.3f  CC4=%5.3f  Total=%5.3f\n", p_C1, p_C2, p_C3, p_C4, p_C_total);
	printf("----- R [=CC_total /(AA_total + BB_total-1)]=%10.5f -----\n", ((float)C_total / (float)(A_total + B_total - 1)));

	fprintf(fpw, "AA1=%5.3f\tAA2=%5.3f\tAA3=%5.3f\tAA4=%5.3f\tTotal=%5.3f\n", p_A1, p_A2, p_A3, p_A4, p_A_total);
	fprintf(fpw, "BB1=%5.3f\tBB2=%5.3f\tBB3=%5.3f\t\tTotal=%5.3f\n", p_B1, p_B2, p_B3, p_B_total);
	fprintf(fpw, "CC1=%5.3f\tCC2=%5.3f\tCC3=%5.3f\tCC4=%5.3f\tTotal=%5.3f\n", p_C1, p_C2, p_C3, p_C4, p_C_total);
	fprintf(fpw, "----- R [=CC_total /(AA_total + BB_total-1)]=%10.5f -----\n", ((float)C_total / (float)(A_total + B_total - 1)));
}

void ins(short a[], short c[], short NN)
{
	short i;
	for (i = 0; i < NN; i++)
		c[i] = a[i];
}

void str_extract(char s[], int start, int str_len, char ss[]) {
	int i;
	for (i = 0; i < str_len; i++) {
			ss[i] = s[start + i];
	}
	ss[i] = '\0';
}
