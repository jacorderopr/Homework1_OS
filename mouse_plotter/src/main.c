#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRAIL_DURATION 3  // Duration in seconds before clearing the screen

int main() {
    FILE* fptr;
    int *numbers;
    long file_size;
    size_t num_integers;
    int x, y;

    // Open the file for reading in binary mode
    fptr = fopen("./mouse_data_shortcut", "rb");
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    file_size = ftell(fptr);
    rewind(fptr);

    // Calculate the number of integers (coordinates)
    num_integers = file_size / sizeof(int);

    // Ensure we have an even number of integers (pairs of x and y)
    if (num_integers % 2 != 0) {
        fprintf(stderr, "Error: File contains an odd number of integers.\n");
        fclose(fptr);
        return 1;
    }

    // Allocate memory for the coordinates
    numbers = (int*)malloc(file_size);
    if (numbers == NULL) {
        perror("Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    // Read the integers from the file
    size_t items_read = fread(numbers, sizeof(int), num_integers, fptr);
    if (items_read != num_integers) {
        fprintf(stderr, "Error reading data from file.\n");
        free(numbers);
        fclose(fptr);
        return 1;
    }

    // Close the file
    fclose(fptr);

    // Initialize ncurses
    initscr();
    noecho();
    curs_set(FALSE);

    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);

    // Scaling factors to map coordinates to terminal size
    float scale_x = (float)max_col / 100.0;
    float scale_y = (float)max_row / 25.0;

    // Start plotting coordinates
    for (size_t i = 0; i < num_integers; i += 2) {
        // Get the x and y coordinates
        x = numbers[i];
        y = numbers[i + 1];

        // Scale the coordinates to fit the terminal size
        int term_x = (int)(x * scale_x);
        int term_y = (int)(y * scale_y);

        // Ensure the coordinates are within terminal bounds
        if (term_x < 0) term_x = 0;
        if (term_x >= max_col) term_x = max_col - 1;
        if (term_y < 0) term_y = 0;
        if (term_y >= max_row) term_y = max_row - 1;

        // Plot the asterisk at the scaled coordinates
        mvprintw(term_y, term_x, "*");

        // Refresh the screen to show the new asterisk
        refresh();

        // Wait for 10 milliseconds before plotting the next coordinate
        usleep(10000);  // 10 milliseconds
        if(i % 20 == 0){
            clear();
        }
        
    }

    // Clear the screen
    clear();
    refresh();
    

    // Clean up
    endwin();
    free(numbers);

    return 0;
    }