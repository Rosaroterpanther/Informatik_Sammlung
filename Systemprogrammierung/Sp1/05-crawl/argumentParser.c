#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "argumentParser.h"

int amountArguments = 0;
int amountOptions = 0;

char* command;
char** arguments;
char** options;


int initArgumentParser(int argc, char* argv[]){
  // <command> [arguments..] [options ..]
  if(argc < 0 || argv == NULL){
    errno = EINVAL;
    return -1;
  }

  for(int i=1; i < argc; i++){
    if(argv[i][0] == '-'){
      amountOptions += 1;
    }else{
      amountArguments +=1;
    }
  }

  command = argv[0];
  char* temp_arguments[amountArguments];
  char* temp_options[amountOptions];

  int cur_argument = 0;
  int cur_option = 0;
  for(int i=1; i < argc; i++){
    if(argv[i][0] == '-'){
      temp_options[cur_option] = argv[i];
      cur_option += 1;
    }else{
      temp_arguments[cur_argument] = argv[i];
      cur_argument += 1;
    }
  }

  arguments = temp_arguments;
  options = temp_options;

  return 0;
}

char* getCommand(void){
  return command;
}

char* getValueForOption(char* keyName){
  // "-key=value"
  //TODO Fix Segmentation Fault ...
  for(int i=0; i < amountOptions; i++){
    if(strstr(options[i], keyName) != NULL){
      /*printf("\n");
      char* value = strtok(options[i],"=");
      value = strtok(NULL,"=");*/
      return strstr(options[i], keyName);
    }
  }
  return NULL;
}

int getNumberOfArguments(void){
  return amountArguments;
}

char* getArgument(int index){
  return arguments[index];
}

/*
int main(int argc, char const *argv[]) {
  initArgumentParser(argc,(char**) argv);
  printf("Command: %s\n", getCommand());
  printf("Arguments:\n");
  printf("getNumberOfArguments: %d\n", getNumberOfArguments());
  for(int i=0; i < getNumberOfArguments(); i++){
    printf("Argument[%d]: %s\n",i, getArgument(i));
  }
  printf("Options:\n");
  printf("%s\n", getValueForOption("i"));
  //printf("%s\n", getValueForOption("from"));
  return 0;
}
*/
