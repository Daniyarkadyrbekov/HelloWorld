#define _GNU_SOURCE
#define SIZE 100
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Poland_Expression{
    char* operand;
    struct Poland_Expression* next;
};

struct Poland_Expression* Pop(struct Poland_Expression* head){
    struct Poland_Expression* temp = NULL;
    if(head != NULL){
        temp = head->next;
        free(head->operand);
        free(head);
    }else{
        printf("[error1]\n");
    }
    
    return temp;
}

char* Top(struct Poland_Expression* head){
    if(!head){
        return NULL;
    }
    
    return head->operand;
}

struct Poland_Expression* Add( struct Poland_Expression* head, char* data){
    struct Poland_Expression* temp = (struct Poland_Expression*)malloc(sizeof(struct Poland_Expression));
    temp->operand = (char*)malloc((strlen(data)+1) * sizeof(char));
    if( temp == NULL || temp->operand == NULL ){
        printf("[error]");
        return 0;
    }
    
    strcpy(temp->operand, data);
    temp->next = head;
    
    return temp;
}

struct Poland_Expression* Add_Back( struct Poland_Expression* last, char* data){
    struct Poland_Expression* temp = (struct Poland_Expression*)malloc(sizeof(struct Poland_Expression));
    temp->operand = (char*)malloc((strlen(data)+1) * sizeof(char));
    if( temp == NULL || temp->operand == NULL ){
        printf("[error]");
        return 0;
    }
    
    strcpy(temp->operand, data);
    while(last->next != NULL){
        last = last->next;
    }
    struct Poland_Expression* p = last->next;
    last->next = temp;
    temp->next = p;
    
    return temp;
}

void Clearing(struct Poland_Expression* head){
    while(head != NULL){
        struct Poland_Expression* tmp = head->next;
        free(head->operand);
        free(head);
        head = tmp;
    }
}

int optype(char c){
    switch(c){
        case '|':
            return 0;
            break;
        case '+':
            return 1;
            break;
        case '-':
            return 2;
            break;
        case '*':
            return 3;
            break;
        case '/':
            return 4;
            break;
        case '(':
            return 5;
            break;
        case ')':
            return 7;
            break;
        case '#':
            return 6;
            break;
    }
    return -1;
}

int Is_Number(char c){
    int isnumber = 0;
    if(isdigit(c) || c == '.'){
        isnumber = 1;
    }
    return isnumber;
}

int Is_Unar_Minus(char* expression, int i){
    if(expression[i] == '-'){
        if(i == 0){
            return 1;
        }else{
            if(!Is_Number(expression[i-1])){
                return 1;
            }
        }
    }
    return 0;
}


struct Poland_Expression* Expression_To_Poland_Parser(char* s){
    int d[7][8] = { {4, 1, 1, 1, 1, 1, 1, 5},
        {2, 2, 2, 1, 1, 1, 5, 2},
        {2, 2, 2, 1, 1, 1, 2, 2},
        {2, 2, 2, 2, 2, 1, 5, 2},
        {2, 2, 2, 2, 2, 1, 5, 2},
        {5, 1, 1, 1, 1, 1, 1, 3},
        {2, 2, 2, 2, 2, 1, 5, 2} };
    
    struct Poland_Expression* poland, *temp;
    poland = NULL;
    temp = NULL;
    char tmpChar[] = "|";
    
    temp = Add(temp, tmpChar);
    unsigned long len = strlen(s);
    s[len++] = '|';
    
    int i = 0;
    for( i = 0; i < len; i++ ){
        if(Is_Unar_Minus(s, i)){
            s[i] = '#';
        }
    }
    for( i = 0; i < len; i++ ){
        if( Is_Number(s[i]) ){
            int count = 1;
            char* src = s  + i;
            while(Is_Number(s[i+1])){
                i++;
                count++;
            }
            char number[count + 1];
            strncpy(number, src, count);
            number[count] = '\0';
            if(poland == NULL){
                poland = Add(poland, number);
            }else{
                Add_Back(poland, number);
            }
        }else{
            int rowIndex = optype( *Top(temp) );
            while(d[ rowIndex ][optype(s[i])] == 2){
                Add_Back(poland, Top(temp));
                temp = Pop(temp);
                rowIndex = optype( *Top(temp) );
            }
            if(d[rowIndex][optype(s[i])] == 1){
                char operation[2];
                operation[0] = s[i];
                operation[1] = '\0';
                temp = Add(temp, operation);
            }
            if(d[rowIndex][optype(s[i])] == 3){
                temp = Pop(temp);
            }
            if(d[rowIndex][optype(s[i])] == 5){
                printf("[error]\n");
                exit(0);
            }
        }
    }
    
    Clearing(temp);
    
    return poland;
}

int Is_space(char c){
    if(c == ' ' || c == '\t' || c == '\n'){
        return 1;
    }else{
        return 0;
    }
}

int Space_Deleting(char* line, size_t bytes){
    int i =0;
    
    while(line[i] != '\0'){
        if(Is_space(line[i])){
            memmove(line + i, line + i + 1, bytes - i);
        }else{
            i++;
        }
        
    }
    
    int length = (int)strlen(line) + 1;
    
    return length;
}

int Brackets_Check(char* line){
    int counter = 0;
    int i = 0;
    
    while(line[i] != '\0'){
        if(line[i] == '('){
            counter++;
        }
        if(line[i] == ')'){
            counter--;
            if(counter < 0){
                return 0;
            }
        }
        i++;
    }
    
    return counter == 0 ? 1 : 0;
}

double Solve(struct Poland_Expression* expression){
    double stack[SIZE];
    int i = 0;
    
    while(Top(expression) != NULL){
        if(Is_Number(*Top(expression)) ){
            stack[i++] = atof(Top(expression));
            expression = Pop(expression);
        }else{
            int operation = optype(*Top(expression));
            
            switch (operation) {
                case 1:
                    stack[i - 2] = stack[i - 2] + stack[i - 1];
                    break;
                case 2:
                    stack[i - 2] = stack[i - 2] - stack[i - 1];
                    break;
                case 3:
                    stack[i - 2] = stack[i - 2] * stack[i - 1];
                    break;
                case 4:
                    if(stack[i - 1] == 0){
                        printf("[error]\n");
                        exit(0);
                    }
                    stack[i - 2] = stack[i - 2] / stack[i - 1];
                    break;
                case 6:
                    stack[i - 1] *= -1;
                    i++;
                default:
                    break;
            }
            i--;
            expression = Pop(expression);
        }
    }
    
    return stack[0];
}

int main(){
    char *line = NULL;
    size_t size = 0;
    ssize_t bytes = getline(&line, &size, stdin);
    
    if (line == NULL)
    {
        printf("[error]");
        return 0;
    }
    
    if (bytes == -1)
    {
        free(line);
        printf("[error]");
        return 0;
    }
    
    
    Space_Deleting(line, bytes);
    
    if(!Brackets_Check(line)){
        printf("[error]");
        free(line);
        return 0;
    }
    
    struct Poland_Expression* polandExpression = NULL;
    
    polandExpression = Expression_To_Poland_Parser(line);
    
    double result = Solve(polandExpression);
    
    printf("%.2f", result);
    
    free(line);
    
    return 0;
}
