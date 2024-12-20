#include "mouse_plotter.h"

// Global variables to keep track of plotting state
static int *global_numbers = NULL;
static int global_num_integers = 0;
static int current_index = 0;
static float scale_x = 1.0, scale_y = 1.0;
static int max_row = 0, max_col = 0;
static timer_t plot_timer;


int ReadMouseData(const char *file_path, int **numbers, int *num_integers)
{
  FILE *fptr;
  long file_size;

  fptr = fopen(file_path, "rb");
  if (fptr == NULL)
  {
    perror("Error opening file");
    return 1;
  }

  fseek(fptr, 0, SEEK_END);
  file_size = ftell(fptr);
  rewind(fptr);

  *num_integers = file_size / sizeof(int);

  if (*num_integers % 2 != 0)
  {
    fprintf(stderr, "Error: File contains an odd number of integers.\n");
    fclose(fptr);
    return 1;
  }

  *numbers = (int *)malloc(file_size);
  if (*numbers == NULL)
  {
    perror("Memory allocation failed");
    fclose(fptr);
    return 1;
  }

  int items_read = fread(*numbers, sizeof(int), *num_integers, fptr);
  if (items_read != *num_integers)
  {
    fprintf(stderr, "Error reading data from file.\n");
    free(*numbers);
    fclose(fptr);
    return 1;
  }

  fclose(fptr);

  return 0;
}


void TimerHandler(int signum, siginfo_t *si, void *uc) 
{

    (void)signum;
    (void)si;
    (void)uc;

    if (current_index >= global_num_integers)
    {

        struct itimerspec stop = {0};
        timer_settime(plot_timer, 0, &stop, NULL);
        clear();
        refresh();
        endwin();
        exit(0);
    }


    int x = global_numbers[current_index];
    int y = global_numbers[current_index + 1];
    current_index += 2;

    
    int term_x = (int)(x * scale_x);
    int term_y = (int)(y * scale_y);

    if (term_x < 0)
        term_x = 0;
    if (term_x >= max_col)
        term_x = max_col - 1;
    if (term_y < 0)
        term_y = 0;
    if (term_y >= max_row)
        term_y = max_row - 1;


    mvprintw(term_y, term_x, "*");
    refresh();


    if (current_index % 40 == 0) 
    {
        clear();
    }
}

void PlotMouseData(int *numbers, int num_integers)
{
 
    global_numbers = numbers;
    global_num_integers = num_integers;


    initscr();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, max_row, max_col);

  
    scale_x = (float)max_col / 100.0;
    scale_y = (float)max_row / 25.0;

   
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = TimerHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        perror("sigaction");
        endwin();
        exit(1);
    }

    
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &plot_timer;
    if (timer_create(CLOCK_REALTIME, &sev, &plot_timer) == -1)
    {
        perror("timer_create");
        endwin();
        exit(1);
    }

    // Start the timer to fire every 10 milliseconds
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 10 * 1000000; // 10 ms
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 10 * 1000000; // 10 ms

    if (timer_settime(plot_timer, 0, &its, NULL) == -1)
    {
        perror("timer_settime");
        endwin();
        exit(1);
    }

  
    while (1)
    {
        pause(); 
    }
}
