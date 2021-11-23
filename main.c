#include <argp.h>
#include "headers.h"

int exit_status = EXIT_SUCCESS;

/* FIXME : Modify the following for argp and remove this comment */
const char *argp_program_version = "";
const char *argp_program_bug_address = "";
static char doc[] = "";

/* Unused arguments description*/
static char args_doc[] = "";

static struct argp_option options[] = {

/* FIXME : menu structure goes here and remove this comment */

};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch(key) {

/* FIXME : switch cases will go here to match menu structure and remove this comment */

        case ARGP_KEY_NO_ARGS:
            if(!state->argv[1])
                argp_usage(state);
            return(1);
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return(0);
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[])
{
    /* Ensure the user invoked us with at least 3 arguments */
    if (argc < 3 || strlen(argv[1]) < 1)
    {
        printError(argv[1], -1,"Usage: %s filename hex_start", argv[0]);
        return EXIT_FAILURE;
    }

/* FIXME: Remove comment wrapper once above struct and switch are populated
    argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, 0);
*/

    int start_new;
    sscanf(argv[2], "%06X", &start_new);
    TRECORD* first = readFile(argv[1], &start_new);

    TRECORD* cur = first;
    while(cur) {
        TRECORD* link = cur;
        cur = cur->next;
        free(link->data);
        free(link);
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
