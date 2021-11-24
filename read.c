#include "headers.h"

#define DEBUG


/**
 * Adds a record to the linked list, with memory allocated based on line size
 *
 * @line the line of the file of the record to add to the linked list
 * @line_num pointer to the variable tracking the line number
 * @first reference to the pointer tracking the first record in the linked list
 * @last reference to the pointer tracking the last record in the linked list
 *
 * @return a pointer to the new TRECORD struct in the linked list
 */
TRECORD* addRecord(char *line, int *line_num, TRECORD** first, TRECORD** last)
{
    TRECORD* trec;

    trec = nmalloc(sizeof(TRECORD));
    trec->size = strlen(line)+1;
    trec->data = nmalloc(trec->size * sizeof(char));
    trec->line = *line_num;
    if(!*first)
        *first = trec;
    else
        (*last)->next = trec;
    *last = trec;
    return(trec);
}

/**
 * Reads a sic object file and loads the text records into a linked list
 *
 * @filename filename to the sic object file, relative or absolute name
 * @start_new the new start address for the program to be relocated to
 *
 * @return a pointer to the first TRECORD struct in the linked list
 */
TRECORD* readFile(char* filename, int* start_new)
{
    FILE *fp = NULL;
    TRECORD* first = NULL;
    TRECORD* last = NULL;
    int TREC_LEN = 71;
    int line_num = 0;
    int maddress = 0;
    int mod_len = 0;
    int prog_len = 0;
    int start_old = 0;
    int exec_old = 0;
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

    // get t-records, build linked list
    while (fgets(line, TREC_LEN, fp))
    {
        line_num++;
        // handle text record 84 = T
        if(line[0]==84) {
            TRECORD* trec;

            trec = addRecord(line, &line_num, &first, &last);
            strncpy(trec->data, line, trec->size);
            printf("%s", trec->data);
        }
        // handle modification record 77 = M
        else if(line[0]==77) {
            char *skip;

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&maddress);
            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%02X",&mod_len);
            rewriteSICTRecord(first->next, start_old, *start_new, maddress, mod_len);

#ifdef DEBUG
            printf("mod-address = %X , len = %X \n", maddress, mod_len);
#endif
        }
        // handle header record 72 = H
        else if(line[0]==72) {
            TRECORD* trec;
            char *skip;

            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%06X",&start_old);         // store old start address
            skip = line + RECORD_SIZE_OFFSET * 2;
            sscanf(skip,"%06X",&prog_len);          // store program length

            trec = addRecord(line, &line_num, &first, &last);
            // copy first part of header, H + NAME
            strncpy(trec->data, line, RECORD_SIZE_OFFSET);
            // add the new start + same length to header
            skip = trec->data + RECORD_SIZE_OFFSET;
            sprintf(skip,"%06X%06X\n",*start_new,prog_len);
#ifdef DEBUG
            printf("start old = %X , prog len = %X \n", start_old, prog_len);
#endif
        }
        // handle end record 69 = E
        else if(line[0]==69) {
            TRECORD* trec;
            char *skip;

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&exec_old);

            trec = addRecord(line, &line_num, &first, &last);
            // calculate new address of first line to execute
            sprintf(trec->data,"E%06X\n",(*start_new + (exec_old - start_old)));
#ifdef DEBUG
            printf("exec old = %X\n", exec_old);
#endif
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
