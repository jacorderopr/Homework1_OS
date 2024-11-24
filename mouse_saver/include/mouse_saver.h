#ifndef MOUSE_SAVER_H
#define MOUSE_SAVER_H

#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern FILE *fptr;
extern int fd;

/**
 * @brief Signal handler for SIGINT to close files and exit.
 *
 * @param signal_num Signal number.
 */
void SigIntHandler(int signal_num);

/**
 * @brief Sets up the signal handler for SIGINT.
 */
void SetupSignalHandler();

/**
 * @brief Opens the mouse device file.
 *
 * @param device_path Path to the mouse device.
 * @return int Returns file descriptor on success, -1 on failure.
 */
int OpenMouseDevice(const char *device_path);

/**
 * @brief Opens the output data file.
 *
 * @param file_path Path to the output file.
 * @return FILE* Returns file pointer on success, NULL on failure.
 */
FILE *OpenOutputFile(const char *file_path);

/**
 * @brief Processes mouse events and writes data to file.
 */
void ProcessMouseEvents();

#endif // MOUSE_SAVER_H
