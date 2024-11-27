#ifndef MOUSE_PLOTTER_H
#define MOUSE_PLOTTER_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/**
 * @brief Reads the mouse data from the file.
 *
 * @param file_path Path to the input data file.
 * @param numbers Pointer to store the allocated array of numbers.
 * @param num_integers Pointer to store the number of integers read.
 * @return int Returns 0 on success, non-zero on failure.
 */
int ReadMouseData(const char *file_path, int **numbers, int *num_integers);

/**
 * @brief Plots the mouse data using ncurses.
 *
 * @param numbers Array of integers containing the mouse coordinates.
 * @param num_integers Number of integers in the array.
 */
void PlotMouseData(int *numbers, int num_integers);

/**
 * @brief Handles the timer signal to plot coordinates in the terminal.
 *
 * This function is invoked whenever the timer signal is generated. It processes
 * a global array of integers containing pairs of coordinates, scales the coordinates
 * to fit the terminal window, and plots them as points. If all coordinates have been 
 * plotted, the function stops the timer, clears the terminal, and terminates the program.
 *
 * @param signum The signal number 
 * @param si Pointer to a siginfo_t structure providing additional information about the signal 
 * @param uc Pointer to the user context passed to the signal handler 
 */

void TimerHandler(int signum, siginfo_t *si, void *uc);


#endif // MOUSE_PLOTTER_H
