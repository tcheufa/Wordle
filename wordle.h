#ifndef WORDLE_H
#define WORDLE_H


#define WORD_LENGTH (5)
#define DEBUG 1

/**
 * @brief Represents a wordle game.
 */
typedef struct Wordle_t Wordle;

/**
 * @brief Starts a wordle game, using the word files as arguments.
 *
 * @param answers_file Path to the file containing the possible answers.
 * @param guesses_file Path to the file containing the accepted words.
 * @param answer The chosen answer (NULL if it must be selected at random).
 *
 * @return The game state.
 */
Wordle *wordleStart(char *answers_file, char *guesses_file, char *answer);

/**
 * @brief Frees a given wordle game.
 *
 * @param game Game state.
 */
void wordleFree(Wordle *game);

/**
 * @brief Check if a guess is allowed and compute its pattern.
 *
 * @param game Game state.
 * @param guess Word to guess.
 *
 * @return NULL if the guess is invalid, or the pattern
 * 		   obtained with this word otherwise.
 */
char *wordleCheckGuess(Wordle *game, char *guess);

/**
 * @brief Gets the current word to guess of the game.
 *
 * @param game Game state.
 *
 * @return The true word to guess.
 */
char *wordleGetTrueWord(Wordle *game);

/**
 * @brief Computes the pattern of a guess compared to the true word.
 *
 * @param guess Word tried.
 * @param answer Word to guess.
 *
 * @return A pattern as string of 5 chars (+ the ending '\0'): '_' for
 *         grey, '*' for yellow, 'o' for green
 *
 */
char *wordleComputePattern(char *guess, char *answer);

#endif
