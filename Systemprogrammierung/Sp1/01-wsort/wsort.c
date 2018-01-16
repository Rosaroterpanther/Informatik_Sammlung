//
//  wsort.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 102

/**
 base: Zeiger auf das erste Element
 nel: Anzahl der Elemente im zu sortierenden Feld
 width: Größe eines Elements
 compare: Vergleichsfunktion
 
void qsort(void *base, size_t nel, size_t width, int (*compare) (const void *, const void *));
 
 
 compare
 <0 falls Element 1 kleiner als Element 2
 0 falls Element 1 und Element 2 gleich gewertet
 >0 falls Element 1 groeßer als Element 2
 **/

static int wrap_stringPointerCompare(const void *a, const void *b){
    const char **x = (const char **)a;
     const char **y = (const char **)b;
    return strcmp(*x, *y);
}

int main(int argc, char* argv[]){
    
    //Test stdin
    if(ferror(stdin) != 0){
        fprintf(stderr, "Fehler ferror stdin"); //Notwendig? errno wird nicht gesetzt
        exit(EXIT_FAILURE);
    }
    
    int NUMBER_OF_STRINGS = 20;
    char **words;
    char buf[LINE_LENGTH];
    
    //Init place for string pointers
    words = malloc(NUMBER_OF_STRINGS * sizeof(char *));
    if(words == NULL){
        perror("Fehler Malloc init words");
        exit(EXIT_FAILURE);
    }
    
    //Init place for each string
    for(int i=0; i < NUMBER_OF_STRINGS; i++){
        words[i] = malloc(sizeof(char[LINE_LENGTH]));
        if(words[i] == NULL){
            perror("Fehler Malloc init words[i]");
            exit(EXIT_FAILURE);
        }
    }

    /**
        get word list
        char *fgets(char *s, int n, FILE *fp)
        fp == stdin
        n-1 Zeichen oder \n oder EOF
        s wird mit \0 abgeschlossen \n nicht entfernt
        bei EOF oder Fehler wird nULL zurueck gegeben und errno sonst s
    **/
    
    int n = 0;
    while(fgets(buf, LINE_LENGTH, stdin) != NULL){
        size_t cur_length = strlen(buf);
        
        //Auf zu langes Wort pruefen
        if(cur_length == 101 && buf[100] != '\n'){
            fprintf(stderr, "String buf is too long");

            int cur_char;
            cur_char = getchar();
            while(cur_char != EOF && cur_char != '\n'){
                cur_char = getchar();
                continue;
            }
        }
        
        // replac \n at the end of line with \0
        if(buf[cur_length-1] == '\n'){
            buf[cur_length-1] = '\0';
            cur_length--;
        }
        
        // Check empty line
        if( cur_length < 1){
            //empty line just ignore
            continue;
        }
        
        if(n >= NUMBER_OF_STRINGS-1){
            //resize words
            NUMBER_OF_STRINGS = NUMBER_OF_STRINGS + 20;
            char **newWords = realloc(words, NUMBER_OF_STRINGS * sizeof(char *));
            if(newWords == NULL){
                perror("Fehler Realloc words");
                free(words);
                exit(EXIT_FAILURE);
            }
            words = newWords;
            for(int i= n + 1; i < NUMBER_OF_STRINGS; i++){
                words[i] = malloc(sizeof(char[LINE_LENGTH]));
                if(words[i] == NULL){
                    perror("Malloc reInit words[i]");
                    exit(EXIT_FAILURE);
                }
            }
               
        }
        
        strcpy(words[n], buf);
        
        n = n + 1;
    }
               
    //sort words
    qsort(words, n, sizeof(*words), wrap_stringPointerCompare);
    
    //Test stdout
    if(ferror(stdout) != 0){
        fprintf(stderr, "Fehler ferror stdout"); //Auch hier notwendig?
        exit(EXIT_FAILURE);
    }
               
    //Liste schreiben
    // int fputs(char *s, FILE *fp)
    //fp == stdout oder stderr
    //return anzahl geschriebener Zeichen
    
    for(int i=0; i < n; i++){
        if(words[i] != NULL){
            puts(words[i]);
            free(words[i]);
        }
    }

    free(words);
               
    return EXIT_SUCCESS;
}
