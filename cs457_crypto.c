#include "cs457_crypto.h"

/* just for generate random nums */
#include <time.h>

int getRandomNumber(int upper)
{
    srand(time(NULL));
    int num = rand() % upper;
    return num;
}

int *generateKey(int size)
{
    int key[size];
    for (int i = 0; i < size; i++)
    {
        key[i] = getRandomNumber(RANDOM_UPPER + i) % RANDOM_UPPER;
    }
    return key;
}

char *initBuffer(char *s)
{
    for (int i = 0; i < _MAX_SIZE; i++)
        s[i] = ' ';

    return s;
}

void testFunction(char *s, int size)
{
    /* printf("before\n");

    for (int i = 0; i < size; i++)
        printf(" %c ", s[i]);
 */
    int messageNumbers[size];
    printf("\n");
    /* translate string to int */
    for (int i = 0; i < size; i++){
        messageNumbers[i] = (int)s[i];
        printf(" %d " , messageNumbers[i]);
    }
    printf("\n");

    int *keyNumbers = generateKey(size);

    char *keyString;
    for (int i = 0; i < size; i++)
    {
        keyString[i] = keyNumbers[i] + '0';
        printf(" %d ", keyNumbers[i]);
    }
    char* destString = malloc(size);
    for(int i =0; i < size; i++)
    {
        destString[i] = (keyString[i] + s[i]) % 26;
        destString[i] += '0';
    }
    
    printf("After\n");
    printf("-----------%s---------\n", destString);
    return;
}

int main(int argc, char **argv)
{
    FILE *file;
    if (argc == 1)
    {
        printf("i need a TestFile \n");
    }
    else
    {
        file = fopen(argv[1], "r");
        if (!file)
        {
            fprintf(stderr, "cannot open file\n");
            return EXIT_FAILURE;
        }
        char *chunk = malloc(_MAX_SIZE);
        chunk = initBuffer(chunk);

        while (fgets(chunk, sizeof(chunk), file) != NULL)
        {
            testFunction(chunk, sizeof(chunk));
        }

        fclose(file);
    }
    return 0;
}