/****************************************************************************
 * main.c
 *
 * Application Security, Assignment 1
 *
 * Abhijit Chitnis (aac664) - func prototypes from code written by Ben Halperin
 ***************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int main(int argc, char** argv)
{
    // declare hashtable_t hashtable[]
    // declare char *misspelled[] to store misspelled words
    // open the input file - pass FILE * to check_words
    // load dictionary
    // call check_words

    hashmap_t hashtable[HASH_SIZE];
    char * misspelled[MAX_MISSPELLED];

    // Assuming main() does not declare these arrays
    // hashmap_t hashtable[0];
    // char * misspelled[0];

    int num_misspelled = 0;
    char * dictionary_file = "./wordlist.txt";

    FILE * fp = fopen("./input_file.txt", "r");
    if (fp == NULL){
        printf("Could not open input_file.txt to be spell checked\n");
	exit(1);
    }
    else {
	printf("Opened input_file.txt successfully\n");
    }

    if (load_dictionary(dictionary_file, hashtable)){
	num_misspelled = check_words(fp, hashtable, misspelled);
    }
    else{
	printf("Failed to load dictionary file - exiting without checking input file\n"); 
	exit(1);
    }

    printf("Total misspelled words = [%d]\n", num_misspelled);
    for (int i=0; i < num_misspelled; i++){
	printf("[%d]: %s\n", i+1, misspelled[i]);
    }

    exit(0);
}
