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
	char *string;
	unsigned int length;
	int numberScore;
	int scrabbleScore;
	struct Word *nextWordPointer;
	struct Word *lastWordPointer;
}Word;

void printHelp(void);
unsigned char parseFlags(int argc, const char *argv[]);
Word *findWords(int argc, const char *argv[], const char flags);
Word *newWord(char *word);
void printList(Word *word);
void printList_r(Word *word);
Word *insertNewWord(Word *word, char *string, const char flags);
int stringcmp(char *wordString, char *string);

int main(int argc, const char *argv[])
{
	unsigned char flags = 0;
	flags = parseFlags(argc, argv);
	if((flags & ALL_SORT) == 0)
	{
		flags |= A_FLAG;
	}
	Word *tree = findWords(argc,argv, flags);
	if((flags & R_FLAG) == R_FLAG)
	{
		printList_r(tree);
	}
	else
	{
		printList(tree);
	}
//	freeWordList(tree);

	return 0;
}

void printHelp(void)
{
	printf("Help Here.\n");
}

unsigned char parseFlags(int argc, const char *argv[])
{
	int rFlagCount = 0;
	unsigned char flags = 0;
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

Word *findWords(int argc, const char *argv[], const char flags)
{
	FILE *currFile;
	int multiplier = 1;
	int buffSize = MAX_SIZE;
	char *buff = calloc((buffSize * multiplier), 1);
	if(buff == NULL)
	{
		printf("Out of memory. Quitting.\n");
		exit(2);
	}
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
					printf("Could not open file: %s\n", argv[i]);
				}
				int j = 0;
				while(fgets(buff, (buffSize * multiplier), currFile) != NULL)
				{
					if(strlen(buff) == (unsigned )(buffSize * multiplier) - 1)
					{
						multiplier++;
						buff = realloc(buff, (buffSize * multiplier)+ 1);	
						fseek(currFile, -strlen(buff), SEEK_CUR);						
						continue;
					}
					if(j == 0)
					{
						list = insertNewWord(list, buff, flags);
						j++;
					}
					else
					{
						insertNewWord(list, buff, flags);
					}
				}
				fclose(currFile);
				free(buff);
			}
		}
	}
	return list;
}

Word *newWord(char *word)
{
	Word *new = malloc(sizeof(Word));
	word[strlen(word)] = 0;
	if(new == NULL)
	{
		printf("Out of Memory. Quitting.\n");
		exit(2);
	}
	int sum = 0;
	if(new == NULL)
	{
		printf("Unable to add to list. Quitting.\n");
		exit(2);
	}
	new->string = malloc(strlen(word) + 1);
	if(new->string == NULL)
	{
		printf("Out of Memory. Quitting.\n");
		exit(2);
	}
	strcpy(new->string, word);
	new->length = strlen(word);
	for(unsigned int i = 0; i < strlen(word); i++)
	{
		sum += word[i];	
	}
	new->numberScore = sum;
	new->nextWordPointer = new->lastWordPointer = NULL;
	return new;
}

void printList(Word *word)
{
	if(word != NULL)
	{
		printList(word->lastWordPointer);
		printf("%s", word->string);
		printList(word->nextWordPointer);
		free(word->string);
		free(word);
	}
}

void printList_r(Word *word)
{
	if(word != NULL)
	{
		printList_r(word->nextWordPointer);
		printf("%s", word->string);
		printList_r(word->lastWordPointer);
	}
}

Word *insertNewWord(Word *word, char *string, const char flags)
{
	if(word == NULL)
	{
		return newWord(string);
	}
	int score = 0;
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		score += string[i];	
	}
	if((flags & A_FLAG) == A_FLAG && stringcmp(word->string, string))
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
	}
	else if((flags & A_FLAG) == A_FLAG)
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
	}
	else if(((flags & L_FLAG) == L_FLAG) && word->length < strlen(string))
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
	}
	else if(((flags & L_FLAG) == L_FLAG))
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
	}
	else if(((flags & N_FLAG) == N_FLAG) && word->numberScore < score)
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
	}
	else if(((flags & N_FLAG) == N_FLAG))
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
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
	else if(strlen(string) > strlen(wordString))
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
