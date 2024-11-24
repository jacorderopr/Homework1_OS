#include "mouse_plotter.h"

int main()
{
    int *numbers = NULL;
    int num_integers = 0;

    const char *input_file = "./mouse_data_shortcut";

    if (ReadMouseData(input_file, &numbers, &num_integers) != 0)
    {
        return 1;
    }

    PlotMouseData(numbers, num_integers);

    free(numbers);

    return 0;
}
