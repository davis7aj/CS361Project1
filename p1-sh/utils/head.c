#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// You may assume that lines are no longer than 1024 bytes
#define LINELEN 1024

static void usage (void);

int
main (int argc, char *argv[])
{
  FILE *head = fopen (argv[argc - 1], "r");
  char chunk[2048];

  if (head == NULL)
    {
      return 1;
    }

  if (argc == 4 && strcmp (argv[1], "-n") != 0)
    {
      return EXIT_FAILURE;
    }

  if (strcmp (argv[1], "-n") == 0)
    {
      for (int i = 0; i < atoi (argv[2]); i++)
        {
          if (fgets (chunk, 2048, head) != NULL)
            {
              printf ("%s", chunk);
            }
        }
    }
  else
    {
      int i = 0;
      while (fgets (chunk, 2048, head) != NULL && !(i >= 5))
        {
          printf ("%s", chunk);
          i++;
        }
    }
  fclose (head);
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("head, prints the first few lines of a file\n");
  printf ("usage: head [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -n N     show the first N lines (default 5)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
