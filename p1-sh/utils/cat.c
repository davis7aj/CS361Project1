#include <stdio.h>
#include <stdlib.h>

static void usage (void);

int
main (int argc, char *argv[])
{
      if (argc < 1)
  {
    usage();
    return 1;
  }
  FILE *cat = fopen (argv[argc - 1], "r");
  char chunk[2048];

  if (cat == NULL)
    {
      return 1;
    }
  while (fgets (chunk, 2048, cat) != NULL)
  {
    printf("%s", chunk);
  }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("cat, print the contents of a file\n");
  printf ("usage: cat FILE\n");
}
