#include <stdio.h>
#include <stdlib.h>

#define C_FLAG 		0x01
#define R_FLAG 		0x02
#define N_FLAG 		0x04
#define L_FLAG 		0x08
#define S_FLAG	 	0x10
#define A_FLAG	 	0x20
#define U_FLAG	 	0x40
#define H_FLAG		0x80
#define ALL_SORT	0x3C

void printHelp(void);
unsigned char parseFlags(int argc, const char *argv[]);

int main(int argc, const char *argv[])
{
	unsigned char flags = parseFlags(argc, argv);
	printf("%d\n", flags);
	return 0;
}

void printHelp(void)
{
	printf("Help Here.\n");
}

unsigned char parseFlags(int argc, const char *argv[])
{
	int rFlagCount = 0;
	unsigned char flags;
	if(argc > 1)
	{
		for(int i = 0; i < argc; i++)
		{
			if(*argv[i] == '-')
			{
				switch(argv[i][1])
				{
					case('r'):
						rFlagCount++;
						break;
					case('n'):
						flags &= ~ALL_SORT;
						flags |= N_FLAG;
						break;
					case('l'):
						flags &= ~ALL_SORT;
						flags |= L_FLAG;
						break;
					case('s'):
						flags &= ~ALL_SORT;
						flags |= S_FLAG;
						break;
					case('a'):
						flags &= ~ALL_SORT;
						flags |= A_FLAG;
						break;
					case('u'):
						flags |= U_FLAG;
						break;
					case('h'):
						printHelp();
						exit(0);
					default:
						printf("Unknown Switch present.\n");
						exit(1);

				}
			}
		}
		if(rFlagCount % 2 == 1)
		{
			flags |= R_FLAG;
		}
	}
	else
	{
	}
	return flags;
}
