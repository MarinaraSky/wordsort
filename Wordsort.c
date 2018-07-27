#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_FLAG 		0x01
#define R_FLAG 		0x02
#define N_FLAG 		0x04
#define L_FLAG 		0x08
#define S_FLAG	 	0x10
#define A_FLAG	 	0x20
#define U_FLAG	 	0x40
#define ALL_SORT	0x3C
#define MAX_SIZE	128

typedef struct Word 
{
	char string[MAX_SIZE];
	int isChunk;
	int length;
	int numberScore;
	int scrabbleScore;
	struct Word *nextWordPointer;
	struct Word *lastWordPointer;
}Word;

void printHelp(void);
unsigned char parseFlags(int argc, const char *argv[]);
void findFileNames(int argc, const char *argv[]);
Word *newWord(const char *word);
void alphaSort(Word *word);
void alphaSort_r(Word *word);
Word *insertNewWord(Word *word, char *string);
int stringcmp(char *wordString, char *string);
void freeWordList(Word *word);

int main(int argc, const char *argv[])
{
	unsigned char flags = parseFlags(argc, argv);

	findFileNames(argc, argv);
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
	return flags;
}

void findFileNames(int argc, const char *argv[])
{
	FILE *currFile;
	char buff[MAX_SIZE];
	Word *list = NULL;
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(*argv[i] != '-')
			{
				currFile = fopen(argv[i], "r");
				if(currFile == NULL)
				{
					//fail case not sure yet
				}
				int j = 0;
				while(fgets(buff, MAX_SIZE - 1, currFile) != NULL)
				{
					if(j == 0)
					{
						list = insertNewWord(list, buff);
						j++;
					}
					else
					{
						insertNewWord(list, buff);
					}
				}
				fclose(currFile);
			}
		}
		alphaSort(list);
		printf("\n");
		alphaSort_r(list);
		freeWordList(list);
	}
}

Word *newWord(const char *word)
{
	Word *new = malloc(sizeof(Word));
	strcpy(new->string, word);
	new->nextWordPointer = new->lastWordPointer = NULL;
	return new;
}

void alphaSort(Word *word)
{
	if(word != NULL)
	{
		alphaSort(word->lastWordPointer);
		printf("%s", word->string);
		alphaSort(word->nextWordPointer);
	}
}

void alphaSort_r(Word *word)
{
	if(word != NULL)
	{
		alphaSort_r(word->nextWordPointer);
		printf("%s", word->string);
		alphaSort_r(word->lastWordPointer);
	}
}

Word *insertNewWord(Word *word, char *string)
{
	if(word == NULL)
	{
		return newWord(string);
	}
	if(stringcmp(word->string, string))
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string);
	}
	else
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string);
	}
	return word;
}

int stringcmp(char *wordString, char *string)
{
	int returnCode = 0;
	if(strlen(string) < strlen(wordString))
	{
		for(unsigned int i = 0; i < strlen(string); i++)
		{
			if(string[i] < wordString[i])
			{
				returnCode = 1;
				break;
			}		
			else if(string[i] > wordString[i])
			{
				break;
			}
		}
	}
	else
	{
		for(unsigned int i = 0; i < strlen(wordString); i++)
		{
			if(string[i] < wordString[i])
			{
				returnCode = 1;
				break;
			}		
			else if(string[i] > wordString[i])
			{
				break;
			}
		}
	}
	return returnCode;
}

void freeWordList(Word *word)
{
	if(word == NULL)
	{
		return;	
	}
	if(word->nextWordPointer)
	{
		freeWordList(word->nextWordPointer);
	}
	else
	{
		freeWordList(word->lastWordPointer);
	}
	free(word);
}
