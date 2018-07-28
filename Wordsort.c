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
	unsigned int dupe;
	unsigned int length;
	int numberScore;
	int scrabbleScore;
	struct Word *nextWordPointer;
	struct Word *lastWordPointer;
}Word;

static void printHelp(void);
static unsigned char parseFlags(int argc, const char *argv[]);
static Word *findWords(int argc, const char *argv[], const char flags);
static Word *newWord(char *word);
static void printList(Word *word, const char flags);
static void printList_r(Word *word, const char flags);
static Word *insertNewWord(Word *word, char *string, const char flags);
static int stringcmp(char *wordString, char *string);
static int getScrabbleScore(char *string);

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
		printList_r(tree, flags);
	}
	else
	{
		printList(tree, flags);
	}

	return 0;
}

static void printHelp(void)
{
	printf("Usage: ./ws [-h][-c NUM][-r][-n][-l][-a][-u][-s] [FILENAME]\n");
	printf("\n");
	printf("\t-h, --help:\t\tThis menu\n");
	printf("\t-c NUM, --count NUM:\tDisplay NUM amount of lines.\n");
	printf("\t-r, --reverse:\t\tDisplay results in reverse order.\n");
	printf("\t-n, --number:\t\tDisplay results sorted by their number score.\n");
	printf("\t-l, --length:\t\tDisplay results base on length.\n");
	printf("\t-a, --alpha:\t\tDisplay results alphabetically(Default).\n");
	printf("\t-u, --unique:\t\tDisplay results that are unique only.\n");
	printf("\t-s, --scrabble:\t\tDisplay results based on their score in the game Scrabble.\n");
	printf("\n");
	printf("If no parameters are set, the user can input data directly into the terminal.\n"
			"To sort the input press CTL+D.\n");
}

static int cCount = 0;

static unsigned char parseFlags(int argc, const char *argv[])
{
	int rFlagCount = 0;
	unsigned char flags = 0;
	if(argc > 1)
	{
		for(int i = 0; i < argc; i++)
		{
			if(*argv[i] == '-')
			{
				char *args = (char *)argv[i];
				if(strcmp(args, "--help") == 0)
				{
					strcpy(args, "-h");
				}

				else if(strcmp(args, "--count") == 0)
				{
					strcpy(args, "-c");
				}
				else if(strcmp(args, "--reverse") == 0)
				{
					strcpy(args, "-r");
				}
				else if(strcmp(args, "--number") == 0)
				{
					strcpy(args, "-n");
				}
				else if(strcmp(args, "--length") == 0)
				{
					strcpy(args, "-l");
				}
				else if(strcmp(args, "--alpha") == 0)
				{
					strcpy(args, "-a");
				}
				else if(strcmp(args, "--unique") == 0)
				{
					strcpy(args, "-u");
				}
				else if(strcmp(args, "--scrabble") == 0)
				{
					strcpy(args, "-s");
				}
				switch(args[1])
				{
					case('c'):
						if(argc > i + 1 && atoi(argv[i + 1]))
						{
							flags |= C_FLAG;
							cCount = atoi(argv[i + 1]);
						}
						else
						{
							printf("No value for -c. Continuting without.\n");
						}
						break;
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

static Word *findWords(int argc, const char *argv[], const char flags)
{
	FILE *currFile;
	int multiplier = 1;
	int buffSize = MAX_SIZE;
	char *tokens;
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
			currFile = fopen(argv[i], "r");
			if(currFile == NULL)
			{
				continue;
			}
			int j = 0;
			while(fgets(buff, (buffSize * multiplier), currFile) != NULL)
			{
				if(strlen(buff) == (unsigned )(buffSize * multiplier) - 1)
				{
					multiplier++;
					fseek(currFile, -strlen(buff), SEEK_CUR);
					buff = realloc(buff, (buffSize * multiplier)+ 1);	
					continue;
				}
				tokens = strtok(buff, " ");
				while(tokens !=  0)
				{
					if(j == 0)
					{
						list = insertNewWord(list, tokens, flags);
						j++;
					}
					else
					{
						insertNewWord(list, tokens, flags);
					}
					tokens = strtok(NULL, " ");
				}
			}
			fclose(currFile);
			free(buff);
		}
	}
	else
	{
		int j = 0;
		while(fgets(buff, (buffSize * multiplier), stdin) != NULL)
		{
			if(strlen(buff) == (unsigned )(buffSize * multiplier) - 1)
			{
				multiplier++;
				fseek(stdin, -strlen(buff), SEEK_CUR);
				buff = realloc(buff, (buffSize * multiplier)+ 1);	
				continue;
			}
			tokens = strtok(buff, " ");
			while(tokens !=  0)
			{
				if(j == 0)
				{
					list = insertNewWord(list, tokens, flags);
					j++;
				}
				else
				{
					insertNewWord(list, tokens, flags);
				}
				tokens = strtok(NULL, " ");
			}
		}
		free(buff);
	}
	return list;
}

static Word *newWord(char *word)
{
	Word *new = malloc(sizeof(Word));
	word[strlen(word)] = 0;
	if(new == NULL)
	{
		printf("Out of Memory. Quitting.\n");
		exit(2);
	}
	int sum = 0;
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
	new->scrabbleScore = getScrabbleScore(new->string);
	new->numberScore = sum;
	new->nextWordPointer = new->lastWordPointer = NULL;
	return new;
}

static void printList(Word *word, const char flags)
{
	if(word != NULL && cCount >= 0)
	{
		printList(word->lastWordPointer, flags);
		if((flags & U_FLAG) == U_FLAG && (word->dupe & 1) == 1);
		else
		{
			if((flags & C_FLAG) == C_FLAG && cCount >= 1 && word != NULL)
			{
				cCount--;
				printf("%s\n", word->string);
			}
			else if((flags & C_FLAG) != C_FLAG)
			{
				printf("%s\n", word->string);
			}

		}
		printList(word->nextWordPointer, flags);
		free(word->string);
		free(word);
	}
}

static void printList_r(Word *word, const char flags)
{
	if(word != NULL && cCount >= 0)
	{
		printList_r(word->nextWordPointer, flags);
		if((flags & U_FLAG) == U_FLAG && (word->dupe & 1) == 1);
		else
		{
			if((flags & C_FLAG) == C_FLAG && cCount >= 1 && word != NULL)
			{
				cCount--;
				printf("%s\n", word->string);
			}
			else if((flags & C_FLAG) != C_FLAG)
			{
				printf("%s\n", word->string);
			}
		}
		printList_r(word->lastWordPointer, flags);
		free(word->string);
		free(word);
	}
}

static Word *insertNewWord(Word *word, char *string, const char flags)
{
	string[strcspn(string, "\n")] = 0;
	if(word == NULL)
	{
		return newWord(string);
	}
	int score = 0;
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		score += string[i];	
	}
	if((flags & A_FLAG) == A_FLAG && stringcmp(word->string, string) == 1)
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	else if((flags & A_FLAG) == A_FLAG)
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	else if(((flags & L_FLAG) == L_FLAG) && word->length < strlen(string))
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}

	}
	else if((flags & L_FLAG) == L_FLAG)
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}

	}
	else if(((flags & N_FLAG) == N_FLAG) && word->numberScore < score)
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	else if((flags & N_FLAG) == N_FLAG)
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	else if(((flags & S_FLAG) == S_FLAG) && word->scrabbleScore < score)
	{
		word->lastWordPointer = insertNewWord(word->lastWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	else if((flags & S_FLAG) == S_FLAG)
	{
		word->nextWordPointer = insertNewWord(word->nextWordPointer, string, flags);
		if((flags & U_FLAG) == U_FLAG)
		{
			if(strcmp(word->string, string) == 0)
			{
				word->dupe |= 0x1;
			}
			else
			{
				word->dupe = 0;
			}
		}
	}
	
	return word;
}

static int stringcmp(char *wordString, char *string)
{
	int returnCode = 0;
	if(strlen(string) <= strlen(wordString))
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


static int getScrabbleScore(char *string)
{
	int scrabble[] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
	int score = 0;
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		score += scrabble[string[i] - 'a'];
	}
	return score;
}
