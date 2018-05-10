/*
  Anfrage modul
*/

#include "request.h"
#include "cmdline.h"
#include "i4httools.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

static const size_t MAX_REQUEST_LENGTH = 8192;
static const char *wwwPath;

static void sendFile(FILE *client, const char path[], const char relPath[]){
  FILE *file = fopen(path, "r");
	if (file == NULL) {
		httpNotFound(client, path, relPath);
		return;
	}
	httpOK(client);
	int c;
	while ((c = getc(file)) != EOF) {
		if (putc(c, client) == EOF)
			break;
	}
	fclose(file);
}

int initRequestHandler(void){
  wwwPath = cmdlineGetValueForKey("wwwpath");
  if(wwwPath == NULL || strlen(wwwPath) == 0){
    return -1;
  }
  return 0;
}

void handleRequest(FILE *client){

  char line[MAX_REQUEST_LENGTH + 1];
  if(fgets(line, sizeof(line), client) == NULL){
    if(ferror(client) != 0){
      perror("Fehler fgets");
      return;
    }
  }

  if(strncmp(line, "GET", 4) != 0){
    httpBadRequest(client, line);
    return;
  }

  const char *url = strtok(line + 4, " \t\r\n");
  if(url == NULL){
    httpBadRequest(client, line);
    return;
  }
  // Bail out if we have a directory-traversal attack
	char path[strlen(wwwPath) + strlen(url) + 1];
	strcat(strcpy(path, wwwPath), url);
	if (checkPath(url) == -1) {
		httpForbidden(client, path);
		return;
	}

  // Do not attempt to deliver anything other than a regular file
	struct stat status;
	if (stat(path, &status) != 0) {
		if (errno == ENOENT)
			httpNotFound(client, path);
		else
			httpForbidden(client, path);
		return;
	}
	if (S_ISREG(status.st_mode) == 0) {
		httpForbidden(client, path);
		return;
	}

	// Deliver the requested file
	sendFile(client, path, url);

}
