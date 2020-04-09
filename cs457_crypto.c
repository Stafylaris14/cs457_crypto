#include "cs457_crypto.h"

/* just for generate random nums */
#include <time.h>

FILE *urandom;

char charSet[62];

char Capitals[26];
char v_char_set[26][26];
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

int get_capital(char c)
{
    int i = 0;
    for (i = 0; i < 26; i++)
    {
        if (Capitals[i] == c)
            return i;
    }
    return -1;
}
void init_v_char_set()
{
    /* initialise Capitals char set */
    int i, index = 65;
    for (i = 0; i < 26; i++)
    {
        Capitals[i] = index;
        index++;
    }

    int j;
    int index_of_capitals = 0;
    for (i = 0; i < 26; i++)
    {
        for (j = 0; j < 26; j++)
        {
            index_of_capitals = i + j;

            if (index_of_capitals > 25)
                index_of_capitals -= 26;
            v_char_set[i][j] = Capitals[index_of_capitals];
        }
    }
}

void print_v()
{
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
            printf("|%c|", v_char_set[i][j]);

        printf("\n");
    }
}

int getLen(uint8_t *s)
{

    return sizeof((char *)s) / sizeof(uint8_t);
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
    int index, char_in_char_set;
    uint8_t *res = malloc(sizeof(message));
    for (index = 0; index < size; index++)
    {
        char_in_char_set = get_char_index((char)message[index]) + get_char_index((char)key[index]);
        if (char_in_char_set > 62)
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

        if (char_in_char_set < 0)
            char_in_char_set = char_in_char_set + 62;

        message[i] = charSet[char_in_char_set];
    }
    return message;
}

uint8_t *caesar_encrypt(uint8_t *plaintext, int n)
{
    int size = getLen(plaintext);
    uint8_t *res = malloc(size);
    int i, index_in_char_set;
    for (i = 0; i < size; i++)
    {
        index_in_char_set = get_char_index(plaintext[i]) + n;
        while (index_in_char_set > 62)
            index_in_char_set -= 62;
        res[i] = charSet[index_in_char_set];
    }

    res[i] = '\0';
    return res;
}

uint8_t *caesar_decrypt(uint8_t *ciphertext, int n)
{
    int size = getLen(ciphertext);
    uint8_t *res = malloc(size);
    int i, index_in_char_set;
    for (i = 0; i < size; i++)
    {
        index_in_char_set = get_char_index(ciphertext[i]) - n;
        while (index_in_char_set < 0)
            index_in_char_set += 62;
        res[i] = charSet[index_in_char_set];
    }

    res[i] = '\0';
    return res;
}

uint8_t *spartan_encrypt(uint8_t *plaintext, int circ, int len)
{
    int size = getLen(plaintext);

    int i = 0, j = 0, index;

    char tmp_arr[len][circ];
    for (int a = 0; a < circ; a++)
    {
        for (int b = 0; b < len; b++)
        {
            tmp_arr[b][a] = '\\';
        }
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
            /* printf("[%c] ", tmp_arr[i][j]); */
        }
        /* printf("\n"); */
    }

    res[index] = '\0';
    return res;
}

uint8_t *vigenere_encrypt(uint8_t *plaintext, uint8_t *key)
{
    /* to plain text einai to i kai to key to j */
    int i, j, index, size = getLen(plaintext);
    uint8_t *res = malloc(sizeof(plaintext));
    for (index = 0; index < size; index++)
    {
        i = get_capital(plaintext[index]);
        j = get_capital(key[index]);

        res[index] = v_char_set[i][j];
        
    }
    
    res[index] = '\0';
    return res;
}

uint8_t *vigenere_decrypt(uint8_t *ciphertext, uint8_t *key)
{
    int i, j, index, size = getLen(ciphertext);
    uint8_t *res = malloc(sizeof(ciphertext));
    for (index = 0; index < size; index++)
    {
        
        j = get_capital(key[index]);
        for(i = 0; i < 26; i++)
            if(v_char_set[i][j] == ciphertext[index])break;
        res[index] = v_char_set[i][0];
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
    uint8_t message[] = "Eimaipro";
    init_char_set();

    printf("--------otp encryption--------\n");

    uint8_t *key_otp = generateKey(getLen(message));
    uint8_t *crypto_otp = one_time_pad_encrypt(message, key_otp);
    printf("the crypto text -> (%s)\n", crypto_otp);
    printf("ORIGINAL MESSAGE WAS (%s)\n", one_time_pad_decrypt(crypto_otp, key_otp));

    printf("--------caesar encryption--------\n");

    uint8_t *crypto_caesar = caesar_encrypt(message, 100);
    printf("the crypto text caesar is -->(%s)\n", crypto_caesar);
    printf("and the original  message was (%s)\n", caesar_decrypt(crypto_caesar, 100));

    printf("--------spartan--------\n");

    uint8_t *crypto_sparan = spartan_encrypt(message, 2, 4);
    printf("the res text is = (%s)\n", crypto_sparan);
    uint8_t *spartan_plaintext = spartan_decrypt(crypto_sparan, 2, 4);
    printf("the original message was (%s)\n", spartan_plaintext);

    printf("--------spartan--------\n");
    init_v_char_set();

    uint8_t v_message []= "EIMAIPRO";
    uint8_t key_v []= "AAAAAAAZ";
    uint8_t* crypto_v = vigenere_encrypt(v_message , key_v);
    printf("the crypto for v is (%s)\n" , crypto_v);
    printf("the original for v was (%s)\n" , vigenere_decrypt(crypto_v , key_v));

    
    return 0;
}