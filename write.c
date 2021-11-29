
#include "headers.h"

/**
 * Writes a sic object file stored in a linked list to disk
 *
 * @filename filename to the sic object file, relative or absolute name
 * @first a pointer to the first RECORD struct in the linked list
 *
 * @return a  integer status
 */

int writeFile(char* filename, RECORD* first)
{
    FILE fp = NULL;
    RECORD cur;
    char *newfile;

    fp = fopen(newfile, "w");

    if (!fp)
    {
        if (errno != 0)
            printError(filename, -1, "%s", strerror(errno));
        else
            printError(filename, -1, "File not found or could not be written to");
        return NULL;
    }

    // write linked list
    cur = first;
    while(curr)
{
      if(curr->size != 0)
        fprintf(fp, "T%s%s%s\n", cur->start, cur->size, cur->data);
      else
	fprintf(fp, "%s", cur->data);

    cur = cur->next;
}

     fclose(fp);
     return first;
 }
