#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include "helperfunctions.h"

int n;
struct dirent **namelist;
char *path2;

//function to make absolute path for each file
//used before call lstat
char *makePath(char *path, char *filename) {

   if (path[0] == '.')
        return filename;
    char *result = malloc(1000);
    strcpy(result, path);
    if(result[strlen(result) - 1] != '/')
        strcat(result, "/");
    strcat(result, filename);
    return result;
}

//filter directories with .(hidden)
int myFilter(const struct dirent * dir) {
    char *s = dir->d_name;
    if (s[0] == '.')
        return 0;
    return 1;
}

//filter directories with .(hidden)
int filter2(const struct dirent * dir) {
    char *s = dir->d_name;
    if (s[0] == '.')
        return 0;
    return 1;
}

//comparator function to sort by alphabetic order
int alphaCmp(void* a, void* b) {
    return strcasecmp((*(struct dirent **) b)->d_name,
            (*(struct dirent**) a)->d_name);
}

//comparator function to sort by size(size element in dirnet)
int sizeCmp(void* a, void* b) {
    return ((*(struct dirent**) b)->d_reclen - (*(struct dirent**) a)->d_reclen);
}

//comparator function to sort by size(real file size)
int sizeCmp2(void* a, void* b) {
    struct stat s1,s2;

    if (lstat(makePath(path2, (*(struct dirent**) a)->d_name), &s1))
                perror("Error");
    if (lstat(makePath(path2, (*(struct dirent**) b)->d_name), &s2))
                perror("Error");
    return (s1.st_size - s2.st_size);
}

//function to scan specified directory
int scanDirs(char *path, bool all, bool size) {

    path2 = malloc(100);
    strcpy(path2, path);

    if (size && all) {
        if ((n = scandir(path, &namelist, 0, &sizeCmp2)) < 0) {
            perror("\nScandir");
            return -1;
        }
    }
    else if (size) {
        if ((n = scandir(path, &namelist, filter2, &sizeCmp2)) < 0) {
            perror("\nScandir");
            return -1;
        }
    }
    else if (all) {
        if ((n = scandir(path, &namelist, 0, &alphaCmp)) < 0) {
            perror("\nScandir");
            return -1;
        }
    }
    else if ((n = scandir(path, &namelist, &myFilter, &alphaCmp)) < 0) {
        perror("\nScandir");
        return -1;
    }
}

//print result according to the enabled flags
int printDirs(char *path, bool list, bool inode) {
    struct stat filestat;

    if (inode && list) {
        while (n--) {
            if (lstat(makePath(path, namelist[n]->d_name), &filestat))
                perror("Error");

            printf("%7lu %s %s %8lu %s %s\n", namelist[n]->d_ino,
                    make_permissions_string(&filestat),
                    make_user_group_string(&filestat), filestat.st_size,
                    format_time(filestat.st_mtim), namelist[n]->d_name);

            free(namelist[n]);
        }
        return (EXIT_SUCCESS);
    }
    if (inode) {
        while (n--) {
            printf("%7lu  %s\n", namelist[n]->d_ino, namelist[n]->d_name);
            free(namelist[n]);
        }

        free(namelist);
        exit(EXIT_SUCCESS);
    }

    if (list) {
        while (n--) {
            
            if (lstat(makePath(path, namelist[n]->d_name), &filestat))
                perror("Error");

            printf("%s %s %8lu %s %s\n", make_permissions_string(&filestat),
                    make_user_group_string(&filestat), filestat.st_size,
                    format_time(filestat.st_mtim), namelist[n]->d_name);

            free(namelist[n]);
        }
        return (EXIT_SUCCESS);
    }


    while (n--) {
        printf("%s\n", namelist[n]->d_name);
        free(namelist[n]);
    }

    free(namelist);
}

