#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* struct to hold T records */
typedef struct trecord {
    int line;
    int size;
    char* data;
    void* next;
} TRECORD;

/*** main prototypes ***/

/**
 * Allocates memory and null fills the memory
 *
 * @param size the amount of memory to be allocated
 *
 * @return a void pointer to the first address of allocated memory
 */
void* nmalloc(size_t size);

/**
 * Print an error
 *
 * @param filename the filename of the file being parsed
 * @param line the line number of the file the error occurred on
 * @param format a formatted string
 * @param ... variable arguments to match the format (va_list)
 */
void printError(const char *filename, const int line, const char *format, ...);

/**
 * Reads a sic object file and loads the text records into a linked list
 *
 * @filename filename to the sic object file, relative or absolute name
 *
 * @return a pointer to the first TRECORD struct in the linked list
 */
TRECORD* readFile(char* filename);

/**
 * Relocates a t-record to a new start address.
 *
 * @param tRecord the t-record that is going to be relocated
 * @param start the new start address of the program after relocation
 * @param mAddress the address of the address dependent object code
 *
 * @return 1 on successful rewrite of the t-record, otherwise the function returns 0
 */
int rewriteSICTRecord(TRECORD* record, int* start, int mAddress)


