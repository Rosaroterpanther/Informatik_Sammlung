#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argumentParser.h"

void directory_show_content(char* path);

int main(int argc, char const *argv[]) {

  if(argc < 2){
    fprintf(stderr, "Usage: crawl path … [-maxdepth=n] [-name=pattern] [-type=d,f] [-size=[+-]n]\n");
    exit(EXIT_FAILURE);
  }

  if(initArgumentParser(argc, (char**)argv) == -1){
    perror("Fehler initArgumentParser");
    exit(EXIT_FAILURE);
  }

/* TODO
  int depth = 0;
  int maxdepth = atoi(getValueForOption("maxdepth"));
*/

// TODO Options

  for(int i=0; i < getNumberOfArguments(); i++){
    directory_show_content(getArgument(i));
  }


  return 0;
}

void directory_show_content(char* path){
  DIR* cur_dir = opendir(path);
  if(cur_dir == NULL){
    perror("Fehler cur_dir skip");
    return;
  }

  struct dirent* cur_dir_dirent = readdir(cur_dir);
  while(cur_dir_dirent != NULL){
    struct stat cur_stat;
    char c_path[255];
    sprintf(c_path, "%s/%s", path ,cur_dir_dirent->d_name);
    //printf("%s\n",c_path);
    if(stat(c_path, &cur_stat) == -1){
      perror("Fehler stat");
    }else{
      if(S_ISDIR(cur_stat.st_mode) && strcmp(cur_dir_dirent->d_name,".")!=0 && strcmp(cur_dir_dirent->d_name,"..")!=0){
        printf("%s/%s\n", path, cur_dir_dirent->d_name);
        char d_path[255];
        sprintf(d_path, "%s/%s", path, cur_dir_dirent->d_name);
        directory_show_content(d_path);
      }else if(S_ISREG(cur_stat.st_mode) && strcmp(cur_dir_dirent->d_name,".")!=0 && strcmp(cur_dir_dirent->d_name,"..")!=0){
        printf("%s/%s\n", path, cur_dir_dirent->d_name);
      }
    }
    cur_dir_dirent = readdir(cur_dir);
  }
  closedir(cur_dir);
}
