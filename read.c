#include "headers.h"

#define DEBUG

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

            trec = nmalloc(sizeof(TRECORD));
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
        // handle modification record 77 = M
        else if(line[0]==77) {
            char *skip;

            skip = line + RECORD_ADDR_OFFSET;
            sscanf(skip,"%06X",&maddress);
            skip = line + RECORD_SIZE_OFFSET;
            sscanf(skip,"%02X",&mod_len);
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

            trec = nmalloc(sizeof(TRECORD));
            trec->size = strlen(line)+1;
            trec->data = nmalloc(trec->size * sizeof(char));
            // copy first part of header, H + NAME
            strncpy(trec->data, line, RECORD_SIZE_OFFSET);
            // add the new start + same length to header
            skip = trec->data + RECORD_SIZE_OFFSET;
            sprintf(skip,"%06X%06X\n",*start_new,prog_len);
            trec->line = line_num;
            if(!first)
                first = trec;
            else
                last->next = trec;
            last = trec;
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

            trec = nmalloc(sizeof(TRECORD));
            trec->size = strlen(line)+1;
            trec->data = nmalloc(trec->size * sizeof(char));
            // calculate new address of first line to execute
            sprintf(trec->data,"H%06X\n",(*start_new + (exec_old - start_old)));
            trec->line = line_num;
            if(!first)
                first = trec;
            else
                last->next = trec;
            last = trec;
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
