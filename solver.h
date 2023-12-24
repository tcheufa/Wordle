#ifndef SOLVER_H
#define SOLVER_H

/**
 * @brief Represents a solver.
 */
typedef struct Solver_t Solver;

/**
 * @brief Creates a new solver.
 *
 * @param answers_file Path to the file containing
 * the possible answers.
 * @param guesses_file Path to the file containing
 * the accepted words.
 *
 * @return The created solver.
 */
Solver *solverStart(char *answers_file, char *guesses_file);

/**
 * @brief Frees a solver.
 *
 * @param solver Solver to free.
 */
void solverFree(Solver *solver);

/**
 * @brief Get the number of possible answers.
 *
 * @param solver the solver.
 *
 * @return the number of possible answers.
 */
int solverGetNbAnswers(Solver *solver);

/**
 * @brief Update the list of possible answers given a
 * guess and a pattern.
 *
 * @param solver the solver.
 * @param guess the guess.
 * @param pattern the pattern.
 *
 * @return the number of removed possible answers.
 */
int solverUpdate(Solver *solver, char *guess, char *pattern);

/**
 * @brief Get the best guess.
 *
 * @param solver the solver.
 * @param guess buffer to store the guess.
 *
 * @return the score of the best guess.
 */
double solverBestGuess(Solver *solver, char *guess);

#endif
