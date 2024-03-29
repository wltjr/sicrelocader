#include <argp.h>
#include "headers.h"

int exit_status = EXIT_SUCCESS;

char *filename;
char *start;
char *platform;

const char *argp_program_version = "Version 1.0";
const char *argp_program_bug_address = "TeamLoaders@123mail.org";
//static char doc[] = "";

/* Unused arguments description*/
//static char args_doc[] = "ARG1 [FILENAME...], ARG2 [ADDRESS...], ARG3 [PLATFORM...]";

static struct argp_option options[] = {
{0,0,0,0,"Required arguments:"},
{"filename",'f',"FILE",0," Interpret argument as filename or file. "},
{"start",'s',"0000",0," Interpret argument as start address in hex. "},
{"platform",'p',"PLATFORM",0," Interpret argument as platform type - SIC or SICXE. "},
{0,0,0,0,"GNU Options:", 2},
{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch(key) {
	 case 'd': printf (".\n");
        break;
	case 'f':
		filename = state->argv[2];
        	filename = arg;
		 break;
	case 's':
	       	start = state->argv[4];
		start = arg;
        	 break;
	case 'p':
	        platform = state->argv[6];
		platform = arg;
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

static struct argp argp	 =  { options, parse_opt };		//{ options, parse_opt, args_doc,doc };

int main(int argc, char* argv[])
{
   // argp_parse (&argp, argc, argv,  ARGP_NO_EXIT, 0, 0); 
   argp_parse (&argp, argc, argv, 0, 0, 0);

   int Sic_max = 32768;
   int SicXE_max = 1000000;
   int start_new = 0;
   char xe_flag = 0;

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
	start_new = strtol(start, NULL, 16);

  if (strcmp( platform,"SIC" ) == 0)
	{
          if (start_new > Sic_max) // START_NEW SHOULD BE INTERPRETED IN HEX AND SIC_MAX 
          {
           printError(NULL, -1, "Exceeded system memory %d > %d, hex %X > %X", start_new, Sic_max, start_new, Sic_max);
           //fclose(fp);
           return EXIT_FAILURE;
	  }
	}
	else if(strcmp( platform,"SICXE" ) == 0) 
	{
       	   xe_flag = 1;

	   if (start_new > SicXE_max) // SHOULD SIC_XE BE F4240 SENSE START_NEW IS INTERPRETED IN HEX???????????????
            {
              printError(NULL, -1, "Exceeded system memory %d > %d, hex %X > %X", start_new, SicXE_max, start_new, SicXE_max);
              //fclose(fp);
              return EXIT_FAILURE;
	    }
	  }
	else
	{
          printError(NULL, -1, "IVALID PLATFORM TYPE");
          //fclose(fp);
          return EXIT_FAILURE;
	}


    RECORD* first = readFile(filename, &start_new, xe_flag);

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
