#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

// No command line can be more than 100 characters
#define MAXLENGTH 100

void shell(FILE *input)
{
  hash_init(100);
  hash_insert("?", "0");
  char buffer[MAXLENGTH];
  while (1)
  {
    // Print the cursor and get the next command entered
    printf("$ ");
    memset(buffer, 0, sizeof(buffer));
    if (fgets(buffer, MAXLENGTH, input) == NULL)
      break;

    if (input != stdin)
      printf("%s", buffer);

    if (strncmp(buffer, "quit", 4) == 0)
    {
      break;
    }

    if (strncmp(buffer, "echo", 4) == 0)
    {
      echo(&buffer[5]);
    }

    if (strncmp(buffer, "cd", 2) == 0)
    {
            char *new_directory = &buffer[3];
            new_directory[strlen(new_directory) -1 ] = '\0';

      if (chdir(new_directory) != 0)
      {
        perror("chdir() error()");
      }
    }

    if (strncmp(buffer, "pwd", 3) == 0)
    {
      pwd();
    }

if (strncmp(buffer, "which", 5) == 0)
    {

      char cmd[strlen(buffer)];
  strncpy(cmd, buffer, strlen(buffer));

// char *cmd = &buffer;
cmd[strlen(buffer) -1] = '\0';

if (strncmp(&cmd[6], "which ", strlen(cmd)) == 0)
{
  /* get rid of extra space on which cmd */
  cmd[strlen(cmd) -1] = '\0';
}

      
      which(&cmd[6]);
      // printf("\ntesting 0000");
    }

if (strncmp(buffer, "export", 6) == 0)
  {
    export(&buffer[7]);
  }

  if (strncmp(buffer, "unset", 5) == 0)
  {
    unset(&buffer[6]);
  }
  
  


  }
  printf("\n");
  hash_destroy();
}
