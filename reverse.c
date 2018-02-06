/*
 * Avery Guething, Operating Systems 2018
 * 
 */
/* TA-BOT:MAILTO avery.guething@marquette.edu */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define BUFSIZE 100
#define binary 2
#define hex 16
#define octal 8
#define decimal 10

//define Length of Node Here
#define LENGTH 100

char buf[BUFSIZE];
int bufp = 0;
int total = 0;

//structure of Node
struct node {
    int count;
    unsigned int value[LENGTH];
    struct node *next;
};


//personal method to check for spaces
int myspace(char c) {
    if(c == '\n') {
        return 0;
    }
    else {
        return isspace(c);
    }
}
//methods by C Programming Book for getch and ungetch
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
//determination determines the base
int determination(char c) {
    int nextChar;
    int nextNextChar;
    int nextNextNextChar;
    nextChar = c;
    
    //exit once user EOF
    if(nextChar == EOF) { 
        return -1;
    }
    nextNextChar = getch();
    
    if(nextChar == '0')
    {
        if(nextNextChar == 'x') {
            nextNextNextChar = getch();
            if(nextNextNextChar == '\n') {
                printf("ERROR: expected number.\n");;
                exit(EXIT_FAILURE); }
                
                ungetch(nextNextNextChar);   
                
                return hex;
        }
        else if(nextNextChar == 'b') {
            nextNextNextChar = getch();
            if(nextNextNextChar == '\n') {
                printf("ERROR: expected number.\n");
                exit(EXIT_FAILURE); }
                ungetch(nextNextNextChar);    
                return binary;
        }
        else {
            
            ungetch(nextNextChar);
            return octal;   
        }
    }
    if(nextChar < '0' || nextChar > '9') {
        printf("ERROR: expected number.\n");
        exit(EXIT_FAILURE);
    }
    
    ungetch(nextNextChar);
    return decimal;
    
}
int mydigit(char c) {
    if(isalpha(c)) {
        printf("ERROR: expected number.\n");
        exit(EXIT_FAILURE);
    }
    else {
        return isdigit(c);
    }
}

//get int and skip whitespace
int getint(unsigned int *pn) {
    unsigned int c;
    
    
    while(isspace(c = getch())) 
        ;
    
    unsigned int base = determination(c);
    if(base == -1) {
        return -1;   
    }
    
    if(base == 0)
        return 0; 
    if(base == hex) {
        c = getch();
        for(*pn = 0; isdigit(c) || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ; c = getch()) {
            if(isdigit(c)) 
                *pn = base * *pn + (c - '0');
            else if((c >= 'A' && c <= 'F'))
                *pn = base * *pn + (c - 55);
            else if((c >= 'a' && c <= 'f')) {
                *pn = base * *pn + (c - 87);
            }
            else {
                printf("ERROR: expected number.\n");
                exit(EXIT_FAILURE);
            }
           
        }
        ungetch(c);
        return c;
    }
    if(base == octal) {
        c = getch();
        for(*pn = 0; isdigit(c); c = getch()) {
            if(c < '0' || c >'7') {
                printf("ERROR: unknown operator 0xFFFFFFFF\n");
                exit(EXIT_FAILURE);
            }
            *pn = base * *pn + (c - '0');
        }
        ungetch(c);
        return c;
    }
    
    if(base == binary) {
        if(!isdigit(c) && c!= EOF) {
            ungetch(c);
            return 0;
        }
        for(*pn = 0; isdigit(c); c = getch()) {
            *pn = base * *pn + (c - '0');
            if(c < '0' || c >'1') {
                printf("ERROR: unknown operator 0xFFFFFFFF\n");
                exit(EXIT_FAILURE);
            }
        }
        if (c != EOF)
            ungetch(c);
        
        return c;
    }
    
    if(!isdigit(c) && c!= EOF) {
        ungetch(c);
        return 0;
    }
    
    for(*pn = 0; mydigit(c); c = getch()) {
        *pn = base * *pn + (c - '0');
        
    }
    if (c != EOF)
        ungetch(c);
    
    return c;
}
//prints element
int printspecial(struct node* Node) {
    int i;
    
    for(i = Node->count -1; i >= 0; i--) {
        printf("%u\n",Node->value[i]);
    }
    if(Node->next != NULL)
        printspecial(Node->next);
}

//frees space once done with nodes
int freespace(struct node* Node) {
    struct node *current = NULL;
    while((current = Node->next) != NULL) {
        free(Node);
        Node = current;
    }    
}

int main() {
    unsigned int d;
    unsigned int total;
    
    //creates head
    char c;
    struct node *head = NULL; 
    head = (struct node*)malloc(sizeof(struct node));
    if(head == NULL) {
        printf("Error: Out of memory.\n");
        exit(EXIT_FAILURE);
    }
    head-> count = 0;
    head -> next = NULL;
    
    //adds temp node if length is greater than set value
    while(getint(&d) != -1) {
     if(head->count >= LENGTH) {
        struct node* temp = (struct node*)malloc(sizeof(struct node));
        temp->next = head;
        head = temp;
        head->count = 0;
     }
     head->value[head->count] = d;
      head->count++;
     
    }
     printspecial(head);
     freespace(head);
}


