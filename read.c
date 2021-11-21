#include "headers.h"

#define DEBUG

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
    TRECORD* last = NULL;
    int TREC_LEN = 71;
    int HEADER_LEN = 20;
    int line_num = 0;
    char header[20] = {0};
    char line[71] = {0};

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
    
    // get header record
    if(fgets(line, TREC_LEN, fp)) {
        line_num++;
        strncpy(header,line, HEADER_LEN);
    }

    // get t-records, build linked list
    while (fgets(line, TREC_LEN, fp))
    {
        line_num++;
        if(line[0]==84) {
            TRECORD* trec = nmalloc(sizeof(TRECORD));
            trec->size = strlen(line)+1;
            trec->data = nmalloc(trec->size * sizeof(char));
            strncpy(trec->data, line, trec->size);
            trec->line = line_num;
            if(!first)
                first = trec;
            else
                last->next = trec;
            last = trec;
        }
    }

#ifdef DEBUG
    // display linked list
    TRECORD* cur = first;
    while(cur) {
        printf("%s",cur->data);
        cur = cur->next;
    }
#endif

    fclose(fp);
    return first;
}
