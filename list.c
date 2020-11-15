#include <stdio.h>
#include <string.h>
#include "shadosh.h"

Wordlist *append(Wordlist** head_ref, char *w) {
    Wordlist *new_node = (Wordlist*)malloc(sizeof(Wordlist)); /* 1. allocate node */
    Wordlist *last = *head_ref;  /* used in step 5 */

    new_node->word  = w; /* 2. put in the data  */
    new_node->next = NULL; /* 3. This new node is going to be the last node, so make next of it as NULL*/

    if (*head_ref == NULL) { /* 4. If the Linked List is empty, then make the new node as head */
       *head_ref = new_node;
       return new_node;
    }  

    while (last->next != NULL) /* 5. Else traverse till the last node */
        last = last->next;

    last->next = new_node; /* 6. Change the next of last node */
    return last->next;    
}

void print(Wordlist **head_ref) {
    Wordlist *temp = *head_ref;
    printf("List is: ");
    while(temp != NULL) {
        printf(" %s",temp->word);
        temp = temp->next;
    }
    printf("\n");
}

void clean_list(Wordlist **head_ref) {
    Wordlist *current = *head_ref;
    Wordlist *next; 
    
    while (current != NULL)  { 
        next = current->next; 
        free(current); 
        current = next; 
    } 
     
    *head_ref = NULL; /* deref head_ref to affect the real head back in the caller. */
}

char **arrayify(Wordlist **head_ref) {
    int i = 0;
    char **arr = calloc(MAXLEN, sizeof(char*));
    Wordlist *temp = *head_ref;
    while(temp != NULL) {
        arr[i++] = temp->word;
        temp = temp->next;
    }

    return arr;
}

char *stringify(char **arr, int len) {
    char *str = calloc(len, sizeof(char*));
    for (int i=0;i<len;i++) {
        if (arr[i] == NULL)
            break;
        strcat(str, arr[i]);
        strcat(str, " ");
    }
    strcat(str, "\n\0");

    return str;
}

/* void del_node(Wordlist *wl) { */
/*     Wordlist *temp = wl; */
/*     temp->word=temp->next->word; */
/*     temp->next=temp->next->next; */
/* } */

