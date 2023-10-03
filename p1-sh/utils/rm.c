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
  for (int i = 1; i < argc; i++)
  {
    remove(argv[i]);
  }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("rm, remove a file\n");
  printf ("usage: rm FILE\n");
}
