#include "headers.h"

/**
 * Writes a sic object file stored in a linked list to disk
 *
 * @filename filename to the sic object file, relative or absolute name
 * @first a pointer to the first RECORD struct in the linked list
 *
 * @return a  integer status
 /

int writeFile(char filename, RECORD* first)
{
    FILE fp = NULL;
    RECORD cur;
    char *newfile;

    // FIXME: change the filename here to a new filename

    fp = fopen(newfile, "w");
