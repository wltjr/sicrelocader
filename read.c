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
 * @return a pointer to the new RECORD struct in the linked list
 */
RECORD* addRecord(char *line, int *line_num, RECORD** first, RECORD** last)
{
    RECORD* rec;

    rec = nmalloc(sizeof(RECORD));
    rec->size = strlen(line)+1;
    rec->data = nmalloc(rec->size * sizeof(char));
    rec->line = *line_num;
    if(!*first)
        *first = rec;
    else
        (*last)->next = rec;
    *last = rec;
    return(rec);
}

/**
 * Reads a sic object file and loads the text records into a linked list
 *
 * @filename filename to the sic object file, relative or absolute name
 * @start_new the new start address for the program to be relocated to
 * @xe_flag the flag which tells the loader if it is reading SIC or SICXE records, non-zero value indicates SICXE
 *
 * @return a pointer to the first RECORD struct in the linked list
 */
RECORD* readFile(char* filename, int* start_new, char xe_flag)
{
    FILE *fp = NULL;
    RECORD* first = NULL;
    RECORD* last = NULL;
    RECORD* cur = NULL;
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
            RECORD* rec;
            char *skip;

            rec = addRecord(line, &line_num, &first, &last);

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&rec->start);
            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%02X",&rec->len);

            strncpy(rec->data, line, rec->size);
#ifdef DEBUG
            printf("tstart-address = %06X, len = %02X \n", rec->start, rec->len);
#endif
        }
        // handle modification record 77 = M
        else if(line[0]==77) {
            char *skip;

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&maddress);
            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%02X",&mod_len);
            if(!cur)
                cur = first;
            cur = rewriteTRecord(cur, start_old, *start_new, maddress, mod_len, xe_flag);

#ifdef DEBUG
            printf("mod-address = %06X, len = %02X \n", maddress, mod_len);
#endif
        }
        // handle header record 72 = H
        else if(line[0]==72) {
            RECORD* rec;
            char *skip;

            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%06X",&start_old);         // store old start address
            skip = line + RECORD_SIZE_OFFSET * 2;
            sscanf(skip,"%06X",&prog_len);          // store program length

            rec = addRecord(line, &line_num, &first, &last);
            // copy first part of header, H + NAME
            strncpy(rec->data, line, RECORD_SIZE_OFFSET);
            // add the new start + same length to header
            skip = rec->data + RECORD_SIZE_OFFSET;
            sprintf(skip,"%06X%06X\n",*start_new,prog_len);
#ifdef DEBUG
            printf("start old = %06X, prog len = %06X \n", start_old, prog_len);
#endif
        }
        // handle end record 69 = E
        else if(line[0]==69) {
            RECORD* rec;
            char *skip;

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&exec_old);

            rec = addRecord(line, &line_num, &first, &last);
            // calculate new address of first line to execute
            sprintf(rec->data,"E%06X\n",(*start_new + (exec_old - start_old)));
#ifdef DEBUG
            printf("exec old = %06X\n", exec_old);
#endif
        }
    }

    // re-write the start address of T records
    cur = first->next;
    char start[RECORD_SIZE_OFFSET+1] = {0};
    while(cur->data[0]!=69) {
        sprintf(start,"T%06X",(*start_new + (cur->start - start_old)));
        strncpy(cur->data,start,RECORD_SIZE_OFFSET-1);
        cur = cur->next;
    }

    // display linked list
    cur = first;
    while(cur) {
        printf("%s",cur->data);
        cur = cur->next;
    }

    // exit label for error goto
    exit:
    fclose(fp);
    return first;
}
