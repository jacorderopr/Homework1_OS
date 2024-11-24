#include "mouse_saver.h"

int main()
{
    const char *device = "/dev/input/mice";
    const char *output_file = "mouse_data.dat";

    SetupSignalHandler();

    if (OpenMouseDevice(device) == -1)
    {
        return 1;
    }

    if (OpenOutputFile(output_file) == NULL)
    {
        close(fd);
        return 1;
    }

    ProcessMouseEvents();

    close(fd);
    fclose(fptr);

    return 0;
}
