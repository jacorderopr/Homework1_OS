#include <fcntl.h>
#include <signal.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

FILE *fptr;
int fd;

void SigIntHandler(int num) {
    char *msg = "Received SIGINT signal! Closing the mouse device file!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    if (fd != -1) close(fd);
    if (fptr != NULL) fclose(fptr);
    exit(0);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = SigIntHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    const char *device = "/dev/input/mice";  // Generic mouse device file
    fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        return 1;
    }

    // Open binary file used to WRITE
    fptr = fopen("mouse_data.dat", "wb");
    if (fptr == NULL) {
        perror("Error opening file");
        close(fd);
        return 1;
    }

    signed char data[3];

    int total_dx = 0;
    int total_dy = 0;
    printf("Listening for mouse events. Move the mouse or press buttons.\n");

    while (1) {
        if (read(fd, data, sizeof(data)) > 0) {
            int dx = data[1];
            int dy = data[2];

            total_dx += dx;
            total_dy += dy;

            // Screen and terminal dimensions
            float screen_width_pixels = 1024.0;
            float screen_height_pixels = 768.0;
            float terminal_width_units = 100.0;
            float terminal_height_units = 25.0;
       
            float pixels_per_unit_x = screen_width_pixels / terminal_width_units;
            float pixels_per_unit_y = screen_height_pixels / terminal_height_units;

            // Calculate terminal coordinates
            float translated_coords_x = (total_dx / pixels_per_unit_x) + (terminal_width_units / 2);
            float translated_coords_y = (total_dy / pixels_per_unit_y) + (terminal_height_units / 2);

            // Ensure coordinates are within terminal bounds
            if (translated_coords_x < 0) translated_coords_x = 0;
            if (translated_coords_x > terminal_width_units) translated_coords_x = terminal_width_units;
            if (translated_coords_y < 0) translated_coords_y = 0;
            if (translated_coords_y > terminal_height_units) translated_coords_y = terminal_height_units;
 
            int x = (int)translated_coords_x;
            int y = (int)translated_coords_y;

            // Write integers to binary file
            fwrite(&x, sizeof(int), 1, fptr);
            fwrite(&y, sizeof(int), 1, fptr);

            printf("dx: %d, dy: %d, translated_x: %d, translated_y: %d\n", dx, dy, x, y);
        }
    }

    close(fd);
    fclose(fptr);
    return 0;
}