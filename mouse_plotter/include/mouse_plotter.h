#ifndef MOUSE_PLOTTER_H
#define MOUSE_PLOTTER_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

#endif // MOUSE_PLOTTER_H
