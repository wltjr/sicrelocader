#include "headers.h"

int exit_status = EXIT_SUCCESS;

int main(int argc, char* argv[])
{
    /* Ensure the user invoked us with at least 2 arguments */
    if (argc < 2 || strlen(argv[1]) < 1)
    {
        printError(argv[1], -1,"Usage: %s filename", argv[0]);
        return EXIT_FAILURE;
    }

    return exit_status;
}

/**
 * Print an error
 *
 * @param filename the filename of the file being parsed
 * @param line the line number of the file the error occurred on
 * @param format a formatted string
 * @param ... variable arguments to match the format (va_list)
 */
void printError(const char *filename, const int line, const char *format, ...)
{
    printf("\x1b[01m\x1b[31;01mERROR\x1b[0m: ");
    va_list(args);
    va_start(args, format);
    vprintf(format, args);
    if(filename && strlen(filename)>0)
    {
        printf(" (\x1b[35m%s\x1b[0m", filename);
        if(line>=0)
            printf(":\x1b[36m%d\x1b[0m", line);
        printf(")");
    }
    printf("\n");
    exit_status = EXIT_FAILURE;
}

/**
 * Allocates memory and null fills the memory
 *
 * @param size the amount of memory to be allocated
 *
 * @return a void pointer to the first address of allocated memory
 */
void* nmalloc(size_t size)
{
    void* memory = NULL;
    memory = malloc(size);
    memset(memory, '\0', size);
    return memory;
}
