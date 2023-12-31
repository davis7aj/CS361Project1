#define _POSIX_SOURCE

#include <errno.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

#include <sys/wait.h>

// default is 1/ fail for testing purposes
int temp = 1;
// char * const envp[1024];

// No command line can be more than 100 characters
#define MAXLENGTH 100

void
shell (FILE *input)
{
  hash_init (100);
  hash_insert ("?", "0");
  char buffer[MAXLENGTH];
  while (1)
    {
      // Print the cursor and get the next command entered
      printf ("$ ");
      memset (buffer, 0, sizeof (buffer));
      if (fgets (buffer, MAXLENGTH, input) == NULL)
        break;

      if (input != stdin)
        printf ("%s", buffer);

      if (strncmp (buffer, "quit", 4) == 0)
        {
          break;
        }

      if (strncmp (buffer, "echo", 4) == 0)
        {
          temp = echo (&buffer[5]);
        }

      if (strncmp (buffer, "cd", 2) == 0)
        {
          char *new_directory = &buffer[3];
          new_directory[strlen (new_directory) - 1] = '\0';

          if (chdir (new_directory) != 0)
            {
              perror ("chdir() error()");
            }
        }

      if (strncmp (buffer, "pwd", 3) == 0)
        {
          temp = pwd ();
        }

      if (strncmp (buffer, "which", 5) == 0)
        {

          char cmd[strlen (buffer)];
          strncpy (cmd, buffer, strlen (buffer));

          // char *cmd = &buffer;
          cmd[strlen (buffer) - 1] = '\0';

          if (strncmp (&cmd[6], "which ", strlen (cmd)) == 0)
            {
              /* get rid of extra space on which cmd */
              cmd[strlen (cmd) - 1] = '\0';
            }

          temp = which (&cmd[6]);
          // printf("\ntesting 0000");
        }

      if (strncmp (buffer, "export", 6) == 0)
        {
          temp = export(&buffer[7]);
        }

      if (strncmp (buffer, "unset", 5) == 0)
        {
          temp = unset (&buffer[6]);
        }

      // We want "ls" from "./bin/ls xxx"
      if (strncmp (buffer, "./bin/", 6) == 0)
        {
          char cmd_cp[256];
          strcpy (cmd_cp, buffer);

          int num_count = 0;
          // int num_count_pipe = 0;
          // int found_pipe = 0;

          char *token = strtok (cmd_cp, " ");

          while (token != NULL)
            {
              // if (strcmp (token, "|") == 0)
              //   {
              //     num_count--;
              //     num_count_pipe++;
              //     found_pipe = 1;
              //   }
              num_count++;
              token = strtok (NULL, " ");
            }
          strcpy (cmd_cp, buffer);
          cmd_cp[strlen (cmd_cp) - 1] = '\0';
          char *argv[num_count + 1];
          // char *argv_pipe[num_count_pipe + 1];
          token = strtok (cmd_cp, " ");
          int i = 0;
          while (token != NULL)
            {
              if (strcmp (token, "|") == 0)
                {
                  break;
                }
              argv[i] = token;
              i++;
              token = strtok (NULL, " ");
            }
          // if (token != NULL)
          //   {
          //     // found pipe char
          //     i = 0;
          //     while (token != NULL)
          //       {
          //         argv_pipe[i] = token;
          //         i++;
          //         token = strtok (NULL, " ");
          //       }
          //   }

          argv[i] = NULL;

          pid_t pid;
          // pid_t pipe_pid;
          posix_spawn_file_actions_t action;
          // posix_spawn_file_actions_t action_pipe;
          if (posix_spawn_file_actions_init (&action) != 0)
            {
              temp = EXIT_FAILURE;
              return;
            }
          int fd[2];
          // int pipe_fd[2];
          if (pipe (fd) == -1)
            {
              close (fd[0]);
              close (fd[1]);
              temp = EXIT_FAILURE;
              return;
            }
          posix_spawn_file_actions_adddup2 (&action, fd[1], STDOUT_FILENO);
          // if (found_pipe)
          //   {
          //     if (pipe (pipe_fd) == -1)
          //       {
          //         close (fd[0]);
          //         close (fd[1]);
          //         close (pipe_fd[0]);
          //         close (pipe_fd[1]);
          //         temp = EXIT_FAILURE;
          //         return;
          //       }
          // if (posix_spawn_file_actions_init (&action) != 0)
          //   {
          //     temp = EXIT_FAILURE;
          //     return;
          //   }
          //     posix_spawn_file_actions_adddup2 (&action_pipe, STDIN, fd[1]);
          //     posix_spawn_file_actions_adddup2 (&action, pipe_fd[1],
          //                                       STDOUT_FILENO);
          //     posix_spawn_file_actions_addclose (&action_pipe, pipe_fd[0]);
          //     posix_spawn_file_actions_addclose (&action_pipe, pipe_fd[1]);
          //   }
          posix_spawn_file_actions_addclose (&action, fd[0]);
          posix_spawn_file_actions_addclose (&action, fd[1]);

          if (posix_spawn (&pid, argv[0], &action, NULL, argv, NULL) == 0)
            {
              close (fd[1]);

              char buf[2048];
              // if (found_pipe)
              //   {
              //     if (posix_spawn (&pipe_pid, argv_pipe[0], &action,
              //     NULL,
              //                      argv_pipe, NULL)
              //         == 0)
              //       {
              //         memset (buf, 0, sizeof (buf));
              //        while (read (pipe_fd[0], buf, 2048) > 0)
              //       {
              //         printf ("%s", buf);
              //         memset (buf, 0, sizeof (buf));
              //       }
              //       }
              //   }
              // else
              //   {
              memset (buf, 0, sizeof (buf));
              while (read (fd[0], buf, 2048) > 0)
                {
                  printf ("%s", buf);
                  memset (buf, 0, sizeof (buf));
                }
              // }
            }
          // if (found_pipe)
          //   {
          //     close (pipe_fd[0]);
          //     close (pipe_fd[1]);
          //   }
          close (fd[0]);
          int exit_code;
          waitpid (pid, &exit_code, 0);
          temp = exit_code;
          if (temp > 1)
            {
              temp = 1;
            }
          posix_spawn_file_actions_destroy (&action);
        }
    }
  printf ("\n");
  hash_destroy ();
}
