#include <argp.h>
#include "headers.h"

int exit_status = EXIT_SUCCESS;


const char *argp_program_version = "1.0";
const char *argp_program_bug_address = "Team Loaders";
static char doc[] = "";

/* Unused arguments description*/
static char args_doc[] = "";

static struct argp_option options[] = {
{0,'d',0,0,"Show a dot on the screen"},
{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch(key) {
	 case 'd': printf (".\n");
        break;
        case ARGP_KEY_NO_ARGS:
            if(!state->argv[1])
                argp_usage(state);
            return(1);
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return(0);
}

static struct argp argp	= { options, parse_opt }; 	// = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[])
{
/////////////////////////////////////////////////////////////
/*
 int start_address_hex = atoi(argv[2]);
 if (start_address_hex > 8000)				//only true if start address is given in hex
{
	printf("USAGE: ERROR: ADDRESS SIZE TOO LARGE\n");
	//fclose(fp);
	return 0;
}*/
/////////////////////////////////////////////////////////////
/* Ensure the user invoked us with at least 3 arguments */
 /* if (argc < 3 || strlen(argv[1]) < 1)
    {
        printError(argv[1], -1,"Usage: %s filename hex_start", argv[0]);
        return EXIT_FAILURE;
    }
*/
/* FIXME: Remove comment wrapper once above struct and switch are populated
    argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, 0);
*/
    argp_parse (&argp, argc, argv,  ARGP_NO_EXIT, 0, 0);

    int start_new;
    sscanf(argv[2], "%06X", &start_new);
    RECORD* first = readFile(argv[1], &start_new);

    RECORD* cur = first;
    while(cur) {
        RECORD* link = cur;
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
