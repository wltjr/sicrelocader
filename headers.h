#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define RECORD_ADDR_OFFSET 1
#define RECORD_SIZE_OFFSET 7

/* struct to hold H, T, and E records */
typedef struct record {
    int len;
    int line;
    int size;
    int start;
    char* data;
    void* next;
} RECORD;

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
 * @start_new the new start address for the program to be relocated to
 *
 * @return a pointer to the first RECORD struct in the linked list
 */
RECORD* readFile(char* filename, int* start_new);

/**
 * Relocates a t-record to a new start address.
 *
 * @param record the t-record that is going to be relocated
 * @param old_Start the current start address of the program prior to relocation
 * @param new_start the new start address of the program after relocation
 * @param m_address the address of the address dependent object code
 * @param half_bytes the number of half bytes that need to be changed according to the m record
 * @param XE_flag the flag which tells the function if the given record is from an XE machine. None zero value indicates XE
 *
 * @return 1 on successful rewrite of the t-record, otherwise the function returns 0
 */
int rewriteTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes, char XE_flag);


