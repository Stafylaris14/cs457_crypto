#include "cs457_crypto.h"

/* just for generate random nums */
#include <time.h>

FILE *urandom;
char charSet[62];

void init_char_set()
{
    int i = 0;
    char index = (char)65;
    /* kefalaia */
    for (i = 0; i < 26; i++)
    {
        charSet[i] = index;
        index++;
    }
    /* mikra */
    index = 97;
    for (i = 26; i < 52; i++)
    {
        charSet[i] = index;
        index++;
    }

    index = 48;
    for (i = 52; i < 62; i++)
    {
        charSet[i] = index;
        index++;
    }
}

/* returns -1 if not found  :(  */
int get_char_index(char c)
{
    int i;
    for (i = 0; i < 62; i++)
        if (c == charSet[i])
            return i;
    
    return -1;
}

int getLen(uint8_t *s)
{
    
    return sizeof((char*)s) / sizeof(uint8_t);
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
    int index , char_in_char_set;
    uint8_t *res = malloc(sizeof(message));
    for (index = 0; index < size; index++)
    {
        char_in_char_set = get_char_index((char)message[index]) + get_char_index((char)key[index]);
        if(char_in_char_set > 62)
            char_in_char_set = char_in_char_set - 62;


        res[index] = charSet[char_in_char_set];
    }
    res[index] = '\0';
    return res;
}

uint8_t *one_time_pad_decrypt(uint8_t *ciphertext, uint8_t *key)
{
    int size = getLen(key);
    int i;
    int char_in_char_set;
    uint8_t *message = malloc(size);
    for (i = 0; i < size; i++)
    {
        char_in_char_set = get_char_index((char)ciphertext[i]) - get_char_index((char)key[i]);

        if(char_in_char_set < 0)
            char_in_char_set = char_in_char_set + 62;
        
        message[i] = charSet[char_in_char_set];
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
    int i = 0, j = 0, index;

    char tmp_arr[len][circ];
    for (int a = 0; a < circ; a++)
    {
        for (int b = 0; b < len; b++)
        {
            tmp_arr[b][a] = '\\';
        }
        printf("\n");
    }
    for (index = 0; index < size; index++)
    {
        if (i == len)
        {
            j++;
            i = 0;
        }
        tmp_arr[i][j] = plaintext[index];
        i++;
    }

    for (int a = 0; a < circ; a++)
    {
        for (int b = 0; b < len; b++)
        {
            printf(" |%c| ", tmp_arr[b][a]);
        }
        printf("\n");
    }

    uint8_t *res = malloc(size);
    index = 0;
    for (int b = 0; b < len; b++)
    {
        for (int a = 0; a < circ; a++)
        {
            //printf(" |%c| ", tmp_arr[b][a]);
            /*  if (tmp_arr[b][a] == '\\')
                continue; */
            res[index] = tmp_arr[b][a];
            index++;
        }
    }

    return res;
}

uint8_t *spartan_decrypt(uint8_t *ciphertext, int circ, int len)
{
    char tmp_arr[len][circ];
    int size = getLen(ciphertext);
    int i = 0;
    int j = 0;
    int index;
    uint8_t *res = malloc(size);
    for (index = 0; index < size; index++)
    {
        tmp_arr[i][j] = ciphertext[index];
        j++;
        if (j == circ)
        {
            j = 0;
            i++;
        }
    }
    index = 0;
    for (j = 0; j < circ; j++)
    {
        for (i = 0; i < len; i++)
        {
            res[index] = tmp_arr[i][j];
            index++;
            printf("[%c] ", tmp_arr[i][j]);
        }
        printf("\n");
    }

    res[index] = '\0';
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
    uint8_t *message = "Eimaipro";
    init_char_set();
    uint8_t *key_otp = generateKey(getLen(message));
    uint8_t* crypto_otp = one_time_pad_encrypt(message ,key_otp );

    printf("-----otp encryption---\n");

    printf("the crypto text -> (%s)\n" , crypto_otp);

    printf("ORIGINAL MESSAGE WAS (%s)" , one_time_pad_decrypt(crypto_otp , key_otp));




    

    uint8_t *ci = spartan_encrypt(message, 2, 4);

    printf("the res text is = (%s)\n", ci);

    uint8_t *spartan_plaintext = spartan_decrypt(ci, 2, 4);

    printf("the original message was (%s)\n", spartan_plaintext);

    printf("\n");
    return 0;
}