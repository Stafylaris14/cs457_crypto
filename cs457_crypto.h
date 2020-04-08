#include <stdio.h>
#include <stdlib.h>
/* string concat */
#include <string.h>



#define URANDOM_DEVICE "/dev/urandom"



#define _MAX_SIZE 100

#define MAX_RAND 20

/* get Length of a uint8_t */
int getLen(uint8_t *s);

int getRandomNumber();

char getRandomChar();

uint8_t *generateKey(int size);

uint8_t *one_time_pad_encrypt(uint8_t *message, uint8_t *key);

uint8_t *one_time_pad_decrypt(uint8_t *ciphertext, uint8_t *key);

uint8_t *caesar_encrypt(uint8_t *plaintext, int  n);

uint8_t *caesar_decrypt(uint8_t *ciphertext, int  n);

uint8_t *spartan_encrypt(uint8_t *plaintext, int  circ, int  len);

uint8_t *spartan_decrypt(uint8_t *ciphertext, int  circ, int  len);