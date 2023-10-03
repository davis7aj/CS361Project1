#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void usage (void);

int
main (int argc, char *argv[])
{
      if (argc < 1)
  {
    usage();
    return 1;
  }
  DIR *d;
  struct dirent *dir;
  d = opendir (argv[argc - 1]);
  if (d == NULL)
    {
      return 1;
    }
  int a = 0;
  int s = 0;
  for (int i = 0; i < argc; i++)
    {
      if (strcmp (argv[i], "-a") == 0)
        {
          a = 1;
        }
      if (strcmp (argv[i], "-s") == 0)
        {
          s = 1;
        }
      if (strcmp (argv[i], "-sa") == 0)
        {
          s = 1;
          a = 1;
        }
      if (strcmp (argv[i], "-as") == 0)
        {
          s = 1;
          a = 1;
        }
    }
  if (d != NULL)
    {
      while ((dir = readdir (d)) != NULL)
        {
          if (dir->d_name[0] == '.' && !a)
            {
              continue;
            }
          if (s)
            {
              char full_path[2048];
              sprintf (full_path, "%s/%s", argv[argc - 1], dir->d_name);
              struct stat st;
              stat (full_path, &st);
              printf ("%ld %s\n", st.st_size, dir->d_name);
            }
          else
            {
              printf ("%s\n", dir->d_name);
            }
        }
      closedir (d);
    }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("ls, list directory contents\n");
  printf ("usage: ls [FLAG ...] [DIR]\n");
  printf ("FLAG is one or more of:\n");
  printf ("  -a       list all files (even hidden ones)\n");
  printf ("  -s       list file sizes\n");
  printf ("If no DIR specified, list current directory contents\n");
}
