#include "solver.h"
#include "wordle.h"
#include "dict.h"
#include "LinkedList.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <assert.h>


#define PATTERN_COUNT 125 // The number of possible patterns of length 5 that can be made with the 3 symbols: '_', '*' and 'o'


struct Solver_t {
    List *Sa; // Possible answers
    List *Sg; // Valid guesses
    int Ng;   // Number of possible guesses
    int Na;   // Number of possible answers
};


static void expectNotNull(void *p);


Solver *solverStart(char *answers_file, char *guesses_file)
{
    Solver *solver = malloc(sizeof(Solver));
    expectNotNull(solver);

    solver->Sa = llCreateEmpty();
    expectNotNull(solver->Sa);

    solver->Sg = llCreateEmpty();
    expectNotNull(solver->Sg);

    char *word = calloc(WORD_LENGTH + 1, sizeof(char));
    expectNotNull(word);

    char *cpy = NULL;

    Node *n = NULL;

    FILE *f_answers = fopen(answers_file, "r");
    expectNotNull(f_answers);

    solver->Na = 0;
    while(fgets(word, WORD_LENGTH + 1, f_answers)) {
        cpy = calloc(WORD_LENGTH + 1, sizeof(char));
        expectNotNull(cpy);

        strcpy(cpy, word);

        n = llCreateNode(cpy);
        expectNotNull(n);

        llInsertLast(solver->Sa, n);
        solver->Na++;
        fgetc(f_answers); // chomp \n character to get to new line
    }
    if (ferror(f_answers)) {
        fprintf(stderr, "Une erreur est survenue lors de la lecture du ficher %s: %s", answers_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    FILE *f_guesses = fopen(guesses_file, "r");
    expectNotNull(f_guesses);

    solver->Ng = 0;
    while(fgets(word, WORD_LENGTH + 1, f_guesses)) {
        cpy = calloc(WORD_LENGTH + 1, sizeof(char));
        expectNotNull(cpy);

        strcpy(cpy, word);

        n = llCreateNode(cpy);
        expectNotNull(n);

        llInsertLast(solver->Sg, n);
        solver->Ng++;
        fgetc(f_guesses); // chomp \n character to get to new line
    }
    if (ferror(f_guesses)) {
        fprintf(stderr, "Une erreur est survenue lors de la lecture du ficher %s: %s", guesses_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return solver;
}


void solverFree(Solver *solver)
{
    llFreeData(solver->Sa);
    llFreeData(solver->Sg);
    free(solver);
}


int solverGetNbAnswers(Solver *solver)
{
    return (int)llLength(solver->Sa);
}


int solverUpdate(Solver *solver, char *guess, char *pattern)
{
    int pattern_eq (void *a, void *b) {
        char *p = wordleComputePattern((char*)a, guess);
        if (0 == strcmp(p, (char*) b)) {
            free(p);
            return 1;
        }
        free(p);
        return 0;
    }

    int equals (void *a, void *b) {
        return 0 == strcmp((char*)a, (char*)b);
    }

    // Removing chosen guess from the List
    llFilter(solver->Sg, equals, guess, 0, 1);

    return llFilter(solver->Sa, pattern_eq, pattern, 0, 1);
}


double solverBestGuess(Solver *solver, char *guess)
{
    assert(NULL != solver);
    assert(NULL != llHead(solver->Sg));
    assert(NULL != llHead(solver->Sa));
    assert(NULL != guess);

    if (llLength(solver->Sa) == 1) {
        strncpy(guess, llData(llHead(solver->Sa)), WORD_LENGTH);
        return 0.;
    }

    char *bestGuess = NULL;
    char *guess_i = NULL;
    char *answer_i = NULL;
    char *pattern = NULL;
    Dict *patternDict = NULL;
    double bestScore = -1;
    double score = 0;
    int mismatches;

    int pattern_ne(void *a, void *b) {
        char *pattern = wordleComputePattern((char*)a, guess_i);
        if (0 != strcmp(pattern, (char*)b)) {
            free(pattern);
            return 1;
        }
        free(pattern);
        return 0;
    }

    Node *na;
    for (Node *ng = llHead(solver->Sg); ng != NULL; ng = llNext(ng)) {
        score = 0.;
        guess_i = llData(ng);
        patternDict = dictCreate(PATTERN_COUNT);
        expectNotNull(patternDict);

        for (na = llHead(solver->Sa); na != NULL; na = llNext(na)) {
            answer_i = llData(na);
            mismatches = 0;
            pattern = wordleComputePattern(answer_i, guess_i);

            if (dictContains(patternDict, pattern)) {
                free(pattern);
                continue;
            }

            mismatches = llFilter(solver->Sa, pattern_ne, pattern, 1, 0);

            dictInsert(patternDict, pattern, mismatches);

            score += mismatches;

            free(pattern);
        }

        score /= solver->Na;
        if (bestScore < score) {
            bestScore = score;
            bestGuess = guess_i;
        }

        dictFree(patternDict);
    }

    strncpy(guess, bestGuess, WORD_LENGTH);

    return bestScore;
}



static void expectNotNull(void *p) {
    if (p == NULL) {
        perror("Une erreur est survenue :");
        exit(EXIT_FAILURE);
    }
}
