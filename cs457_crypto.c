#include "cs457_crypto.h"

/* just for generate random nums */
#include <time.h>

FILE *urandom;

int getLen(uint8_t *s)
{
    return sizeof(s) / sizeof(uint8_t);
}

int getRandomNumber()
{
    char c = getc(urandom);
    while (c <= MAX_RAND)
    {
        c = getc(urandom);
    }

    return (int)c;
}

char getRandomChar()
{

    char c = getc(urandom);
    while ((c < 65 || c > 90) && (c < 97 || c > 122) /* && (c < 48 || c > 57) */)
        c = getc(urandom);

    return c;
}

uint8_t *generateKey(int size)
{
    uint8_t *key = malloc(size);
    for (int i = 0; i < size; i++)
    {
        key[i] = getRandomChar();
    }
    return key;
}

uint8_t *one_time_pad_encrypt(uint8_t *message, uint8_t *key)
{
    int size = getLen(message);
    int index;
    uint8_t *res = malloc(sizeof(message));
    for (index = 0; index < size; index++)
    {
        res[index] = (char)message[index] + key[index];

        res[index] = res[index] % 50;
        printf("eimai gia to res[] = %d\n", res[index]);
        /* kefalaia */
        if (res[index] < 26)
        {
            res[index] += 65;
        }
        else if (res[index] <= 50)
        {
            res[index] = res[index] % 26;
            res[index] += 97;
        }
    }
    res[index] = '\0';
    return res;
}

uint8_t *one_time_pad_decrypt(uint8_t *ciphertext, uint8_t *key)
{
    int size = getLen(key);
    int i;
    uint8_t *message = malloc(size);
    for (i = 0; i < size; i++)
    {
        message[i] = ciphertext[i] - key[i] % 26;
        message[i] = message[i] % 26;
        message[i] = message[i] + 65;
    }
    return message;
}

uint8_t *caesar_encrypt(uint8_t *plaintext, int n)
{
    int size = getLen(plaintext);
    uint8_t *res = malloc(size);
    int i;
    for (i = 0; i < size; i++)
    {
        res[i] = plaintext[i] + n;
    }

    res[i] = '\0';
    return res;
}

uint8_t *caesar_decrypt(uint8_t *ciphertext, int n)
{
    int size = getLen(ciphertext);
    uint8_t *res = malloc(size);
    int i;
    for (i = 0; i < size; i++)
    {
        res[i] = ciphertext[i] - n;
    }

    res[i] = '\0';
    return res;
}

uint8_t *spartan_encrypt(uint8_t *plaintext, int circ, int len)
{
    int size = getLen(plaintext);
    printf("size = %d\n", size);
    int i = 0, j = 0 , index;
    
    char tmp_arr[len][circ];
    for (index = 0; index < size; index++)
    {
        printf("eimai edw(%c)\n", plaintext[index]);
        tmp_arr[i][j] = plaintext[index];
        i++;
        if (i == len)
        {
            j++;
            i = 0;
        }
    }

    for (int a = 0; a < circ; a++)
    {
        for (int b = 0; b < len; b++)
        {
            printf(" |%c| ", tmp_arr[b][a]);
        }
        printf("\n");
    }
    printf("-------------------\n");
    uint8_t *res =malloc(size);
    index = 0;
    for (int b = 0; b < len; b++)
    {
        for (int a = 0; a < circ; a++)
        {
            //printf(" |%c| ", tmp_arr[b][a]);
            res[index] = tmp_arr[b][a];
            index++;
        }
        
    }

    return res;
}

int main(int argc, char **argv)
{
    urandom = fopen(URANDOM_DEVICE, "rb");
    /*  FILE *file;
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
        

        while (fgets(chunk, sizeof(chunk), file) != NULL)
        {
            testFunction(chunk, sizeof(chunk));
        }

        fclose(file);
    }
 */
    uint8_t *message = "thisames";

    uint8_t *ci = spartan_encrypt(message, 2, 4);

    printf("the res text is = (%s)\n", ci);

    printf("\n");
    return 0;
}