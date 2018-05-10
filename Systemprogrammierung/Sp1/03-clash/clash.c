#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COMMAND_LENGTH_MAX (sysconf(_SC_LINE_MAX))

struct command{
  int background;
  char *command;
  char **argv;
} command;

void print_prompt();
struct command *create_command(char *command_line);

int main(int argc, char *argv[]){
  // INIT
  char command[COMMAND_LENGTH_MAX];
  struct command *cur_cmd;

  while(1){

    print_prompt();

    printf("-1");
    // get user command
    if (fgets(command, COMMAND_LENGTH_MAX, stdin) == NULL){
      perror("Fehler fgets");
      exit(EXIT_FAILURE);
    }

    printf("0");
    cur_cmd = create_command(command);

    pid_t p = fork();
    if(p == -1){
      // fork-Fehler
      perror ("Fehler fork");
  		exit (EXIT_FAILURE);
    } else if (p == 0){
      // Kind Prozess
      // int execvp(const char *file, char *const argv[]); Only return if fails
      execvp(cur_cmd->argv[0], cur_cmd->argv);
      fprintf(stderr,"command %s\n", command);
      perror("Fehler execvp");;
      exit(EXIT_FAILURE);
    } else {
      // Elternprozess
      if(cur_cmd->background){

      }else{
        int status;
        // p ist PID des Kind Prozesses
        if (waitpid (p, &status, WNOHANG) == -1) {
    		    perror ("Fehler warte auf Fordergrundprozesse");
    		    exit (EXIT_FAILURE);
    	  }
        printf("Exitstatus [%s] = %d\n", command, status);
      }
    }
  }
	free(cur_cmd);
  return EXIT_SUCCESS;
}

void print_prompt(){
  int dir_length = 100;
  char *buf = malloc(sizeof(char[dir_length]));
  if(buf == NULL){
    perror("Fehler malloc buf");
    exit(EXIT_FAILURE);
  }

  // allocate a larger buffer if necessary
  while(getcwd(buf, sizeof(char[dir_length])) == NULL){
    if(errno == ERANGE){
      dir_length += 50;
      buf = realloc(buf, sizeof(char[dir_length]));
      if(buf == NULL){
        perror("Fehler realloc buf");
        exit(EXIT_FAILURE);
      }
    }else{
      perror("Fehler getcwd");
      strcpy(buf, "[unknown]");
    }
  }

  fprintf(stderr, "%s:", buf);
  free(buf);
}

struct command *create_command(char *command_line){
  struct command *cmd;
  size_t command_line_length = strlen(command_line);

  if(command_line[command_line_length-1] == '\n'){
    command_line_length -= 1;
    command_line[command_line_length] = '\n';
  }

  printf("1");

  cmd = malloc(sizeof(command));
  if(cmd == NULL){
    perror("Fehler cmd malloc");
    exit(EXIT_FAILURE);
  }
  memcpy(cmd->command, command_line, command_line_length+1);

  printf("2");

  int max_token = 10;
  cmd->argv = malloc(max_token * sizeof(char[100]));
  if(cmd->argv == NULL){
    perror("Fehler cmd->argv malloc");
    exit(EXIT_FAILURE);
  }
  printf("3");
  cmd->argv[0] = strtok(command_line, " \t");
  int token = 0;
  while(cmd->argv[token] != NULL){
    token += 1;
    cmd->argv[token] = strtok(NULL, " \t");
    if(token == max_token-1){
      max_token += 10;
      cmd->argv = realloc(cmd->argv, max_token * sizeof(char[100]));
      if(cmd->argv == NULL){
        perror("Fehler cmd->argv realloc");
        exit(EXIT_FAILURE);
      }
    }
  }

  return cmd;
}
