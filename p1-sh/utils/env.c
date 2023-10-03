#include <errno.h>
#include <spawn.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>


static void usage (void);

int
main (int argc, char *argv[], char *envp[])
{
  if (argc > 1)
  {
    char *child_envp[argc];
    int count_child_envp = 0;
    char *child_args[argc];
    int count_child_args = 0;
    for (int i = 1; i < argc; i++)
    {
      if (strchr(argv[i], '=') != NULL)
      {
        child_envp[count_child_envp++] = argv[i];
      }
      else
      {
        child_args[count_child_args++] = argv[i];
      }
    }
    child_envp[count_child_envp] = NULL;
    child_args[count_child_args] = NULL;
    pid_t pid;
    posix_spawn_file_actions_t action;
    if (posix_spawn_file_actions_init (&action) != 0)
      {
        return 1;
      }
    int fd[2];
    if (pipe (fd) == -1)
      {
        close (fd[0]);
        close (fd[1]);
        return 1;
      }
    posix_spawn_file_actions_addclose (&action, fd[0]);
    posix_spawn_file_actions_adddup2 (&action, fd[1], STDOUT_FILENO);
    posix_spawn_file_actions_addclose (&action, fd[1]);

    if (posix_spawn (&pid, child_args[0], &action, NULL, child_args, child_envp) == 0)
      {
        close (fd[1]);

        char buf[2048];
        memset (buf, 0, sizeof (buf));
        while (read (fd[0], buf, 2048) > 0)
          {
            printf ("%s", buf);
            memset (buf, 0, sizeof (buf));
          }
      }
    close (fd[0]);
    int exit_code;
    waitpid (pid, &exit_code, 0);
    posix_spawn_file_actions_destroy (&action);
  }
  if (argc == 1)
  {
    for (int i = 0; envp[i] != NULL; i++)
    {
      printf("%s\n", envp[i]);
    }
  }

    if (argc < 1)
  {
    usage();
    return 1;
  }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("env, set environment variables and execute program\n");
  printf ("usage: env [name=value ...] PROG ARGS\n");
}

