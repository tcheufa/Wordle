#include "wordle.h"
#include "LinkedList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <error.h>
#include <errno.h>
#include <assert.h>


struct Wordle_t {
    char* trueWord;
    List* guesses;
};

//static void print_each(List* ll) {
//    Node* n = llHead(ll);
//    while(n) {
//        printf("%s (%zu)\n", llData(n), strlen(llData(n));
//        n = llNext(n);
//    }
//}


static unsigned get_random(unsigned limit);


Wordle *wordleStart(char *answers_file, char *guesses_file, char *answer)
{
    const void check_null_on_init_w (void* p) {
        if (p == NULL) {
            perror("Impossible de lancer le jeu:");
            exit(EXIT_FAILURE);
        }
    }

    Wordle *wordle = malloc(sizeof(Wordle));
    check_null_on_init_w(wordle);

    // Loading list of possible guesses

    FILE *f_guesses = fopen(guesses_file, "r");
    check_null_on_init_w(f_guesses);

    wordle->guesses = llCreateEmpty();
    check_null_on_init_w(wordle->guesses);

    Node *node = NULL;
    char* buf = NULL;

    #ifdef DEBUG
        size_t guess_count = 0;
    #endif // DEBUG

    while (1) {

        buf = malloc((WORD_LENGTH + 1) * sizeof(char));
        check_null_on_init_w(buf);

        if (fgets(buf, WORD_LENGTH + 1, f_guesses) == NULL)
            break;

        node = llCreateNode(buf);
        check_null_on_init_w(node);

        llInsertLast(wordle->guesses, node);

        #ifdef DEBUG
            guess_count++;
        #endif // DEBUG

        // Chomp '\n' character
        fgetc(f_guesses);
    }
    if (ferror(f_guesses)) {
        fprintf(stderr, "Une erreur est survenue lors de la lecture du ficher %s: %s", guesses_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // If no `answer` is given, take a word randomly from `answers_file`
    size_t word_count = 0;

    if (answer == NULL) {
        /// TODO: Search inside answers_file
        FILE *f_answers = fopen(answers_file, "r");

        if (f_answers == NULL) {
            fprintf(stderr, "Le fichier %s n'a pas pu être ouvert: %s", answers_file, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // Counting number of words in `guesses_file`
        char c;
        while (1) {
            c = fgetc(f_answers);

            if (c == EOF)
                break;

            if(c == '\n' || c == ' ')
                ++word_count;
        }
        if (ferror(f_answers)) {
            fprintf(stderr, "Une erreur est survenue lors de la lecture du ficher %s: %s", answers_file, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // Getting a random line where to pick the word
        long int random_line = (long int) get_random((unsigned)word_count);

        #ifdef DEBUG
                printf("DEBUG MODE: Number of possible guesses: %zu\n", guess_count);
                printf("DEBUG MODE: Number of possible answers : %zu\n", word_count);
                printf("DEBUG MODE: Random line is %ld\n", random_line);
        #endif // DEBUG

        // Moving cursor to random word to choose as answer for the game
        if (0 != fseek(f_answers, (WORD_LENGTH + 1) * random_line, SEEK_SET)) {
            fprintf(stderr, "Une erreur est survenue lors de la lecture du ficher %s: %s", answers_file, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // reading `answer` inside the wordle object
        wordle->trueWord = malloc((WORD_LENGTH + 1) * sizeof(char));

        fgets(wordle->trueWord, WORD_LENGTH + 1, f_answers);
    }
    else {
        // Verify the integrity of the word's length
        if (strlen(answer) != WORD_LENGTH) {
            printf("Le mot %s n'est pas composé d'exactement %d lettres", answer, WORD_LENGTH);
            exit(EXIT_FAILURE);
        }

        wordle->trueWord = answer;
    }

    return wordle;
}


void wordleFree(Wordle *game) {
    free(game->trueWord);
    llFreeData(game->guesses);
    free(game);
}


char *wordleCheckGuess(Wordle *game, char *guess)
{
    // Checking the correct length
    if (strlen(guess) != WORD_LENGTH)
        return NULL;

    // Filter function
    int equals(void* a, void* b) {
        return 0 == strcmp((char*)a, (char*)b);
    }
    // Checking the validity
    if (0 == llFilter(game->guesses, equals, game->trueWord, 1, 0))
        return NULL;

    return wordleComputePattern(guess, game->trueWord);
}


char *wordleGetTrueWord(Wordle *game) {
    return game->trueWord;
}


char *wordleComputePattern(char *guess, char *answer)
{
    size_t answer_len = strlen(answer) + 1;

    // Se remplit des caractères de `answer` à chaque fois qu'une lettre correspondante a été identifiée
    char completed[answer_len];

    char* pattern = malloc(sizeof(char) * (answer_len));

    size_t i, j;

    // initialisation
    // Default pattern is '_____' and changes will be made where necesssary
    for (i = 0; i < answer_len - 1; i++) {
        pattern[i] = '_';
        completed[i] = '\0';
    }
    completed [answer_len - 1] = '\0'; pattern [answer_len - 1] = '\0';

    for(i = 0; guess[i] != '\0'; i++) {
        for (j = 0; answer[j] != '\0'; j++) {
            if (guess[i] == answer[j]) {
                if (i == j) {
                    completed[i] = guess[i];
                    pattern[i] = 'o';
                    break;
                }
                else if (completed[j] == '\0'){
                    if (guess[i] == answer[i]) {
                        completed[i] = guess[i];
                        pattern[i] = 'o';
                        break;
                    }

                    if (answer[j] != guess[j]) {
                        completed[j] = guess[i];
                        pattern[i] = '*';
                        break;
                    }
                }
            }
        }
    }

    return pattern;
}


static unsigned get_random(unsigned limit)
{
    srand(time(NULL));
    return ((unsigned)((rand() % limit) + 1));
}


