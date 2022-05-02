#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 3; ++i)
    {
        if (!(fork()))
        {
            printf("fork %d\n", i);
            char *args[3] = {"./autoClient", argv[1],  NULL};
            execvp(args[0], args);
        }
    }
    for (size_t i = 0; i < 3; i++)
    {
        wait(NULL);
    }
    
    return 0;
}