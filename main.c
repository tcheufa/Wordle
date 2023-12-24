#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "wordle.h"
#include "solver.h"

/* If equal to 1 some debut messages are printed on the screen */

#define DEBUG 1

/**
 * @brief Structure to store the args of the program.
 */
struct _args
{
    bool use_solver;
    char *answers_file;
    char *guesses_file;
    char *answer;
    char *first_guess;
};

/**
 * @brief Play a game of wordle.
 *
 * @param solverp Set to true if the solver should be used.
 * @param answers_file Path to the file containing the possible answers.
 * @param guesses_file Path to the file containing the accepted words.
 * @param answer The answer to the game (NULL if it must be selected at random).
 * @param first_guess The first guess of the game (only for solver).
 */
static void play(bool solverp, char *answers_file, char *guesses_file,
                 char *answer, char *first_guess);

/**
 * @brief Parse the arguments of the program.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 *
 * @return The parsed arguments.
 */
static struct _args parse_args(int argc, char **argv);

/**
 * @brief Print the usage of the program.
 */
static void usage();

static void play(bool solverp, char *answers_file, char *guesses_file,
                 char *answer, char *first_guess)
{
    printf("-------------------------------------\n");
    printf("Bienvenue au Wordle. \nVous avez 6 essais "
           "pour trouver le mot caché.\n _ : La lettre n'est "
           "pas dans le mot.\n * : la lettre est présente mais "
           "à une autre place.\n o : la lettre est la bonne place.\n");
    printf("-------------------------------------\n");

    Wordle *game = wordleStart(answers_file, guesses_file, answer);

    if (DEBUG)
        printf("DEBUG MODE: Hidden word: %s\n", wordleGetTrueWord(game));

    Solver *solver;
    if (solverp)
        solver = solverStart(answers_file, guesses_file);

    // Game Loop
    char guess[10];
    char *pattern = NULL;
    char solverguess[6];
    solverguess[5] = '\0';
    int nbTrials = 0;

    do
    {
        if (pattern)
            free(pattern);

        printf("Essai %d : ", nbTrials + 1);

        if (solverp)
        {
            if (nbTrials == 0 && first_guess)
                strncpy(solverguess, first_guess, 5);
            else
                solverBestGuess(solver, solverguess);

            printf("%s\n", solverguess);
            pattern = wordleCheckGuess(game, solverguess);

            if (!pattern)
            {
                printf("Le premier mot choisi n'existe pas.\n");
                exit(EXIT_FAILURE);
            }

            nbTrials++;
        }
        else
        {
            while (1)
            {
                scanf("%s", guess);
                pattern = wordleCheckGuess(game, guess);
                if (!pattern)
                {
                    printf("Ce mot est invalide ou n'existe pas.\n");
                    printf("Essai %d : ", nbTrials + 1);
                }
                else
                {
                    nbTrials++;
                    break;
                }
            }
        }

        printf("          %s\n", pattern);
        printf("\n");

        if (solverp)
        {
            solverUpdate(solver, solverguess, pattern);
            if (DEBUG)
                printf("DEBUG MODE: (remaining answers: %d)\n", solverGetNbAnswers(solver));
        }
    } while ((strcmp(pattern, "ooooo") != 0) && (nbTrials < 6));

    if (strcmp(pattern, "ooooo") == 0)
    {
        printf("Partie gagnée ! Le mot a été trouvé en %d essais.\n", nbTrials);
    }
    else
    {
        printf("Partie perdue ! Le mot était : %s\n", wordleGetTrueWord(game));
    }

    free(pattern);
    wordleFree(game);
    if (solverp)
        solverFree(solver);
}

static struct _args parse_args(int argc, char **argv)
{
    struct _args args = {false, NULL, NULL, NULL, NULL};

    if (argc == 1)
    {
        usage();
        exit(EXIT_SUCCESS);
    }

    int i;
    for (i = 1; argc - i > 1; i++)
    {
        if (strcmp(argv[i], "--mode") == 0)
        {
            if (strcmp(argv[++i], "solver") == 0)
            {
                args.use_solver = true;
            }
            else if (strcmp(argv[i], "human") == 0)
            {
                args.use_solver = false;
            }
            else
            {
                fprintf(stderr, "Invalid mode: %s.\n", argv[i]);
                usage();
                exit(EXIT_FAILURE);
            }
        }

        else if (strcmp(argv[i], "--answers-file") == 0)
        {
            args.answers_file = argv[++i];
        }

        else if (strcmp(argv[i], "--guesses-file") == 0)
        {
            args.guesses_file = argv[++i];
        }

        else if (strcmp(argv[i], "--answer") == 0)
        {
            args.answer = argv[++i];
        }

        else if (strcmp(argv[i], "--first-guess") == 0)
        {
            args.first_guess = argv[++i];

            if (strlen(args.first_guess) != 5)
            {
                fprintf(stderr, "Invalid first guess: %s. Should contain 5 letters.\n", args.first_guess);
                usage();
                exit(EXIT_FAILURE);
            }
        }

        else
        {
            fprintf(stderr, "Invalid argument: %s.\n", argv[i]);
            usage();
            exit(EXIT_FAILURE);
        }
    }

    if (i != argc)
    {
        fprintf(stderr, "Invalid arguments.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    return args;
}

static void usage()
{
    fprintf(stderr, "Usage: ./wordle [--mode solver|human]"
                    "[--answers-file answers_file]"
                    "[--guesses-file guesses_file]"
                    "[--answer answer]"
                    "[--first-guess first_guess]\n"
                    "--mode: solver or human\n"
                    "--answers-file: file containing the list of possible answers.\n"
                    "--guesses-file: file containing the list of accepted words.\n"
                    "--answer: set the hidden word.\n"
                    "--first-guess: set the first guess (only for the solver mode).\n");
}

int main(int argc, char **argv)
{
    struct _args args = parse_args(argc, argv);

    if (args.answers_file == NULL)
    {
        fprintf(stderr, "Missing answers file.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    if (args.guesses_file == NULL)
    {
        fprintf(stderr, "Missing guesses file.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    play(args.use_solver, args.answers_file, args.guesses_file,
         args.answer, args.first_guess);

    return 0;
}
