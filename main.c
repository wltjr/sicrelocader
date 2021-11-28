#include <argp.h>
#include "headers.h"

int exit_status = EXIT_SUCCESS;

char *filename;
char *start;
char *platform;

const char *argp_program_version = "Version 1.0";
const char *argp_program_bug_address = "TeamLoaders@123mail.org";
static char doc[] = "";

/* Unused arguments description*/
static char args_doc[] = "";

static struct argp_option options[] = {
{0,0,0,0,"Required arguments:"},
{"filename",'f',"FILE",0,"Name of file"}, // -f, --filename    Name of file... some description
{"start",'s',"0000",0,"Start address in hex"},// -s, --start        Start address in hex... some description
{"platform",'p',"PLATFORM",0," Platform SIC or SICXE."},// -p, --platform    Platform SIC or SICXE... some description
{0,0,0,0,"GNU Options:", 2},
{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch(key) {
	 case 'd': printf (".\n");
        break;
	case 'f':
		filename = state->argv[2];
        	 break;
	case 's':
	       	start = state->argv[4];
        	 break;
	case 'p':
	        platform = state->argv[6];
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
    int Sic_max = 32768;
    int SicXE_max = 1000000;
    int start_new = 0;
    printf("start = %s\n",start);
    start_new = strtol(start, NULL, 16);
///////////////////////////////////////////////////////////////
    if(!filename) {
           printError(NULL, -1, "FILE NOT FOUND");
           return EXIT_FAILURE;
        }
        if(!start) {
           printError(NULL, -1, "START ADDRESS NOT FOUND");
           return EXIT_FAILURE;
        }
        if(!platform) {
           printError(NULL, -1, "No platform mentioned");
           return EXIT_FAILURE;
            }
//////////////////////////////////////////////////////////////////
    if (strcmp( platform,"SIC" ) == 0)
	{
          if (start_new > Sic_max)
          {
           printError(NULL, -1, "Exceeded system memory %d > %d, hex %X > %X", start_new, Sic_max, start_new, Sic_max);
           //fclose(fp);
           return EXIT_FAILURE;
	  }
	}
	else if(strcmp( platform,"SICXE" ) == 0) 
	{
	   if (start_new > SicXE_max)
            {
              printError(NULL, -1, "Exceeded system memory %d > %d, hex %X > %X", start_new, SicXE_max, start_new, SicXE_max);
              //fclose(fp);
              return EXIT_FAILURE;
	    }
	}
	else
	{
          printError(NULL, -1, "USAGE: ERROR: IVALID PLATFORM TYPE");
          //fclose(fp);
          return EXIT_FAILURE;
	}

///////////////////////////////////////////////////////////////////////////////////
	RECORD* first = readFile(argv[1], &start_new);

/* FIXME: uncomment
    writeFile(filename,first);
*/
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
