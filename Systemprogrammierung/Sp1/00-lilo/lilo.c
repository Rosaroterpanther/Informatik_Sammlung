#include <stdio.h>
#include <stdlib.h>

struct node{
  int value;
  struct node *next;
};

struct node *head = NULL;
/*  insertElement(int value)
    value >= 0
    Add value on the end of the list, if this value not already in the list

    Return value:
        Success: value
        Else: -1
*/
int insertElement(int value) {
  if(value < 0){
    return -1;
  }else{
      struct node *newNode;
      newNode = (struct node *) malloc(sizeof(struct node));
      if(newNode == NULL){
          //No perror + exit this is a libary
          //perror("Malloc newNode");
          //exit(1);
          return -1;
      }
      newNode->value = value;
      newNode->next = NULL;
      if(head == NULL){
          //empty list
          head = newNode;
          return value;
      }else{
          //no empty list, add newNode at the end
          if(head->next == NULL){
              //check Head value
              if(head->value == value){
                  free(newNode);
                  return -1;
              }
              
              head->next = newNode;
          }else{
              struct node *tmp = head;
              
              //check all following values
              while(tmp->next != NULL){
                  if(tmp->value == value){
                      free(newNode);
                      return -1;
                  }
                  tmp = tmp->next;
              }
              
              //tmp is the last element
              tmp->next = newNode;
          }
          
          return value;
      }
  }
  return -1;
}
/*  removeElement(void)
    Entnimmt den aeltesten Wert aus der Liste

    Return value:
    Erfolgfall old_value
    Sonst -1
*/

int removeElement(void) {
    if(head == NULL){
        return -1;
    }else{
        int ret = head->value;
        struct node *tmp = head->next;
        free(head);
        head = tmp;
        return ret;
    }
  return -1;
}

 int main (int argc, char* argv[]) {
   printf("insert 47: %d\n", insertElement(47));
   printf("insert 11: %d\n", insertElement(11));
   printf("insert 23: %d\n", insertElement(23));
   printf("insert 11: %d\n", insertElement(11));

   printf("remove: %d\n", removeElement());
   printf("remove: %d\n", removeElement());
   printf("remove: %d\n", removeElement());
     printf("remove: %d\n", removeElement());
     printf("remove: %d\n", removeElement());
     
     printf("insert 47: %d\n", insertElement(47));
     printf("insert 47: %d\n", insertElement(47));
     printf("insert 47: %d\n", insertElement(47));
     printf("insert 11: %d\n", insertElement(11));
     printf("insert 47: %d\n", insertElement(47));
     printf("insert 47: %d\n", insertElement(47));
     printf("insert 47: %d\n", insertElement(47));
     
     printf("remove: %d\n", removeElement());
     printf("remove: %d\n", removeElement());
   exit(EXIT_SUCCESS);
 }
