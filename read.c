#include "headers.h"

/**
 * Reads a sic object file and loads the text records into a linked list
 *
 * @filename filename to the sic object file, relative or absolute name
 *
 * @return a pointer to the first TRECORD struct in the linked list
 */
TRECORD* readFile(char* filename)
{
    FILE *fp = NULL;
    TRECORD* first = NULL;

    // directory check
    struct stat s_stat;
    stat(filename, &s_stat);
    if (S_ISDIR(s_stat.st_mode))
    {
        printError(filename, -1,
                   "Directories are unsupported, files only!");
        return NULL;
    }

    fp = fopen(filename, "r");

    if (!fp)
    {
        if (errno != 0)
            printError(filename, -1, "%s", strerror(errno));
        else
            printError(filename, -1, "File not found or could not be read");
        return NULL;
    }

    /* FIXME: code to process file goes here, ideally a function call,
     *          may move fopen/close into said function
     */

    fclose(fp);
    return first;
}
