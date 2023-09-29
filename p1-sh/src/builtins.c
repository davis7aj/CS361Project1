#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

// Given a message as input, print it to the screen followed by a
// newline ('\n'). If the message contains the two-byte escape sequence
// "\\n", print a newline '\n' instead. No other escape sequence is
// allowed. If the sequence contains a '$', it must be an environment
// variable or the return code variable ("$?"). Environment variable
// names must be wrapped in curly braces (e.g., ${PATH}).
//
// Returns 0 for success, 1 for errors (invalid escape sequence or no
// curly braces around environment variables).
int echo(char *message)
{
  if (message != NULL)
  {
    message[strlen(message) - 1] = '\0';
  }
  char *token = strtok(message, "\\n");
  do
  {
    printf("%s\n", token);
    token = strtok(NULL, "\\n");
  } while (token != NULL);

  return 0;
}

// Given a key-value pair string (e.g., "alpha=beta"), insert the mapping
// into the global hash table (hash_insert ("alpha", "beta")).
//
// Returns 0 on success, 1 for an invalid pair string (kvpair is NULL or
// there is no '=' in the string).
int export(char *kvpair)
{
  if (*kvpair == NULL)
  {
    return 1;
  }
  char kvcopy = *kvpair;
  char key = strtok(kvcopy, "=");
  char value = strtok(kvcopy, NULL);
  if (value == NULL)
  {
    return 1;
  }
  hash_insert(key, value);
  return 0;
}

// Prints the current working directory (see getcwd()). Returns 0.
int pwd(void)
{

  char cwd[256];

  if (getcwd(cwd, sizeof(cwd)) == NULL)
    perror("getcwd() error");
  else
    printf("%s\n", cwd);

  return 0;
}

// Removes a key-value pair from the global hash table.
// Returns 0 on success, 1 if the key does not exist.
int unset(char *key)
{
  return 0;
}

// Given a string of commands, find their location(s) in the $PATH global
// variable. If the string begins with "-a", print all locations, not just
// the first one.
//
// Returns 0 if at least one location is found, 1 if no commands were
// passed or no locations found.
int which(char *cmdline)
{

  char *included = {"which", "cd", "unset", "pwd", "export", "export"};

  int isElementPresent = 1;
  //only want to test part that doesnt say which 
  char *cmd = cmdline[6];

  for (int i = 0; i < 6; i++)
  {
    if (included[i] == cmd)
    {
      isElementPresent = 0;
      break;
    }
  }

  if (isElementPresent == 0)
  {
    printf("%s: dukesh built-in command", cmd);
  } else {

printf(cmd);


  }

  return isElementPresent;
}
