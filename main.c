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

    int num_misspelled = 0;
    int dict_words = 0, misspelled_words = 0;

    // char * dictionary_file = "./wordlist.txt";
    // char * input_file = "./input_file.txt";

    if (argc != 3){
        printf("Exiting: Please run spell_check as $spell_check <input file name> <dictionary file name>\n");
	exit(1);
    }

    // printf("Program Name: %s\n",argv[0]);
    // printf("Input File: %s\n",argv[1]);
    // printf("Dictionary File: %s\n",argv[2]);

    // input file from command line
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("Could not open input file [%s] to be spell checked\n", argv[1]);
	exit(1);
    }
    else {
        printf("Opened input file [%s] to be spell checked sucessfully\n", argv[1]);
    }

    // dictionary file from command line
    if (load_dictionary(argv[2], hashtable)){
	num_misspelled = check_words(fp, hashtable, misspelled);
    }
    else{
	printf("Failed to load dictionary file [%s] -  exiting without checking input file [%s]\n", argv[2], argv[1]); 
	exit(1);
    }

    printf("Total misspelled words = [%d]\n", num_misspelled);
    for (int i=0; i < num_misspelled; i++){
	printf("[%d]: %s\n", i+1, misspelled[i]);
    }

    dict_words = free_dictionary(hashtable);
    printf ("Dictionary hashtable - total words freed [%d]\n", dict_words);

    misspelled_words = free_misspelled_list(misspelled);
    printf ("Total misspelled words freed [%d]\n", misspelled_words);

    if (num_misspelled != misspelled_words)
	printf("Number of misspelled words detected [%d] != [%d] misspelled words freed\n", num_misspelled, misspelled_words);

    fclose(fp);

    exit(0);
}
