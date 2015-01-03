#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "directoryFunctions.h"

//check if flags are valid
bool isValid(char c)
{
    bool valid = false;
    char validchars[] = {'a', 'S', 'l', 'i'};
    int i;
    for(i = 0; i < 4 && !valid; i++)
    {
        if(c == validchars[i])
            valid = true;
    }
    return valid;
}

int main(int argc, char** argv)
{
//check if args correctly entered
    if(argc > 6)
    {
        printf("Usage : ls <Options> directory \n");
        exit(EXIT_FAILURE);
    }

    char *path = malloc(100);
    if (argc > 1 && argv[argc - 1][0] != '-')
        strcpy(path, argv[argc - 1]);
    else strcpy(path, ".");

    int j;
    for(j = 0; j < argc; j++)
    {
        if(argv[j][0] == '-')
            if(!isValid(argv[j][1]))
            {
                printf("ls : invalid option");
                exit(EXIT_FAILURE);
            }
    }

//set the flag bits
    int i;
    bool all = false, size = false, list = false, inode = false;
    for(i = 0; i < argc; i++)
    {
        switch (argv[i][1])
        {
            case 'S':
                size = true;
                break;
            case 'a':
                all = true;
                break;
            case 'l':
                list = true;
                break;
            case 'i':
                inode = true;
                break;
        }
    }
    
    if (scanDirs(path, all, size) < 0)
        exit(EXIT_FAILURE);
    
    if (printDirs(path, list, inode) < 0)
        exit(EXIT_FAILURE);
    
    return (EXIT_SUCCESS);

}



