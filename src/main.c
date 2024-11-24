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
  close(fd);
  fclose(fptr);
  exit(1);
}

int main() {

  struct sigaction sa;
  sa.sa_handler = SigIntHandler;
  sigaction(SIGINT, &sa, NULL);


  const char *device = "/dev/input/mice";  // Generic mouse device file
   fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("Error opening device");
    return 1;
  }

  // open binary file used to WRITE
   fptr = fopen("mouse_data.dat", "w");

  signed char data[3];

  int total_dx = 0;
  int total_dy = 0;
  float translated_coords_x = 50; // assume we start in the center
  float translated_coords_y = 12.5; 
  printf("Listening for mouse events. Move the mouse or press buttons.\n");
  while (1) {
    if (read(fd, data, sizeof(data)) > 0) {
      int left = data[0] & 0x1;
      int right = data[0] & 0x2;
      int middle = data[0] & 0x4;
      int dx = data[1];
      int dy = data[2];

      total_dx += dx;
      total_dy += dy;
    // horizontal: 512 * 2 = 1024
    // vertical 384 * 2 = 768

    // assuming terminal based coordinates of x = [0, 100] and y = [0, 25]


    // horizontal ranges from -512 to 512
    // vertical ranges from -384 to 384

    // for horizontal: 512 / 50 = 10.24   (we use 50 because that would be half the horizontal terminal screen and 512 is half the horizontal actual screen)
    // for vertical: 384 / 12.5 = 32.5    (we use 12.5 because that would be half the vertical terminal screen and 384 is half the vertical actual screen)

      translated_coords_x = translated_coords_x + (dx / 10.24);
      translated_coords_y = translated_coords_y + (dy / 32.5); // 32.5
      fprintf(fptr, "%d\n", (int)translated_coords_x);
      fprintf(fptr, "%d\n", (int)translated_coords_y);
      printf("Left: %d, Right: %d, Middle: %d, dx: %d, dy: %d translated_coords_x: %f, translated_coords_y: %f\n\n", left, right,
             middle, dx, dy, translated_coords_x, translated_coords_y);
    }

  }

  close(fd);
  return 0;
}