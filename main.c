#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdbool.h>



//hashtable implementation - needs elaboration
#define TABLE_SIZE 128

typedef struct {
    char* key;
    int value;
} hashTuple;

typedef struct {
    hashTuple *elements;
    int size;
} table;

int hash_function(char *key){
    int hash = 0;
    int length = strlen(key);
    for(int i = 0; i<length ;i++){
        hash = hash + key[i];
    }
    return hash % TABLE_SIZE;
}

void init_table(table *table) {
    table->elements = (hashTuple *)calloc(TABLE_SIZE, sizeof(hashTuple));
    table->size = TABLE_SIZE;
}

void insert(table *table, char *key, int value) {
    int index = hash_function(key);
    int i = index;
    while (table->elements[i].key != NULL) {
        if (strcmp(table->elements[i].key, key) == 0) {
            table->elements[i].value = value;
            return;
        }
        i = (i + 1) % table->size;
        if (i == index) {
            printf("Table is full.\n");
            return;
        }
    }
    table->elements[i].key = strdup(key);
    table->elements[i].value = value;
}


int lookup(table *table, char *key) {
    int index = hash_function(key);
    int i = index;
    while (table->elements[i].key != NULL) {
        if (strcmp(table->elements[i].key, key) == 0) {
            return table->elements[i].value;
        }
        i = (i + 1) % table->size;
        if (i == index) {
            printf("Key not found.\n");
            return 0.0;
        }
    }
    printf("Key not found.\n");
    return 0;
}
table* Hashtable;

//hashtable implementation ends here

int MAX_EXPR_LEN=257;
typedef enum {
    NONE, //end token of array to understand conclusion,after "%" also can be none
    NUM, //all numbers 
    OP, //+,-,&,|,*
    IDENT, //identifiers,initally zero
    FUNC_CALL, //for all functions in the format function(arg1,arg2)
    LPAR,
    RPAR,
    COMMA,
    ASSIGN,
    AND,
    OR

} TokenType;
typedef struct {
    TokenType type;
    char* value; //string value of the token 
    char *function_name;   // Name of function 
    //Token *arguments;  //array for functions arguments just like initial array of all tokens .?

} Token;

Token tokens[257];
int current_index=0;
int current_token_type;
int *num_tokens = 0;
int old;
int numtoken;

Token create_token(int type, char* value);

 Token* tokenize(char* input,Token tokens[],int* num_tokens) { //input is the given input of user as a whole

    int i = 0; //current array location,incremented after each process {
    char curr_char = input[i];
    
    while (curr_char != '\0' && curr_char != '%') {
        if (isdigit(curr_char)) {
            char* start = input + i; //start of the number (ex. 5 for 5674)
            while (isdigit(curr_char)) { //current is a digit
                i++;
                curr_char = input[i]; 
            }
            int length = input + i - start;
            char* token_str = (char*) malloc(length + 1);
            strncpy(token_str, start, length);
            token_str[length] = '\0'; //(last character of the number,so the number's tokenized length is one more)
            tokens[*num_tokens] = create_token(NUM, token_str);
            (*num_tokens)++;
            //free(token_str);
            
        } else if (isalpha(curr_char)) {
            char* start = input + i;
            while (isalnum(curr_char) && curr_char != ' ') {
                i++;
                curr_char = input[i];
            }
            int length = input + i - start;
            char* token_str = (char*) malloc(length + 1);
            strncpy(token_str, start, length);
            token_str[length] = '\0';
            if (strcmp(token_str, "ls") == 0 || strcmp(token_str, "rs") == 0 || 
                strcmp(token_str, "xor") == 0 || strcmp(token_str, "lr") == 0 || 
                strcmp(token_str, "rr") == 0 || strcmp(token_str, "not") == 0) {
                tokens[*num_tokens] = create_token(FUNC_CALL, token_str);
            } else {
                tokens[*num_tokens] = create_token(IDENT, token_str);
                
            }
           
            //free(token_str);
            (*num_tokens)++;

            
        } else if (curr_char == ',') {
    char* token_str = (char*) malloc(2);
    token_str[0] = curr_char;
    token_str[1] = '\0';
    tokens[*num_tokens] = create_token(COMMA,token_str);
    (*num_tokens)++;
    i++;
    curr_char = input[i];
    //free(token_str);
}     
else if (curr_char == '=') {
    char* token_str = (char*) malloc(2);
    token_str[0] = curr_char;
    token_str[1] = '\0';
    tokens[*num_tokens] = create_token(ASSIGN, token_str);
    (*num_tokens)++;
    i++;
    curr_char = input[i];
    //free(token_str);
}     
        else if (curr_char == '+' || curr_char == '-' || curr_char == '*'  )
                      {
            char* token_str = (char*) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] =create_token(OP, token_str);
             //create token returns a pointer this line assigns the pointers reference token to tokens array
            (*num_tokens)++;
            i++;
            curr_char = input[i];
            //free(token_str);
        }
        else if(curr_char == '(' ){
            char* token_str = (char*) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] =create_token(LPAR, token_str);
             //create token returns a pointer this line assigns the pointers reference token to tokens array
            (*num_tokens)++;
            i++;
            curr_char = input[i];
            //free(token_str);



        }else if(curr_char == ')'){
            char* token_str = (char*) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] =create_token(RPAR, token_str);
             //create token returns a pointer this line assigns the pointers reference token to tokens array
            (*num_tokens)++;
            i++;
            curr_char = input[i];
            //free(token_str);

        }
        else if( curr_char == '&'){
            char* token_str = (char*) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] =create_token(AND, token_str);
             //create token returns a pointer this line assigns the pointers reference token to tokens array
            (*num_tokens)++;
            i++;
            curr_char = input[i];
            //free(token_str);


        }else if( curr_char == '|'){
            char* token_str = (char*) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] =create_token(OR, token_str);
             //create token returns a pointer this line assigns the pointers reference token to tokens array
            (*num_tokens)++;
            i++;
            curr_char = input[i];
            //free(token_str);



        }
        else {
            i++;
            curr_char = input[i];
        }
    }
    for (int i = 0; i < *num_tokens; i++) {
    //Token token = tokens[i];

    
    //printf("Token %d: type=%d, value=%s \n", i, tokens[i].type,tokens[i].value);
    numtoken=*num_tokens;
}
    return tokens;


 }



// Function to create a new token
Token create_token(int type, char* value) {
    Token token;
    // Set the token fields
    token.type = type;
    token.value=value;
    //printf("Creating token of value : ... ...%s\n",token.value);


    return token;
}


int precedence(char *op) {
    printf("precedence called this is string: %s",op);
    if  (strcmp(op, "=") == 0)
        return 6;


    else if (strcmp(op, "(") == 0 || strcmp(op, ")") == 0)
        return 0;

//     else if (strcmp(op, ",") == 0)
//        return 4;

    else if (strcmp(op, "xor") == 0 || strcmp(op, "not") == 0 || strcmp(op, "ls") == 0 || strcmp(op, "rs") == 0 || strcmp(op, "lr") == 0 || strcmp(op, "rr") == 0)
        return 5;

    else if (strcmp(op, "*") == 0){
    printf("multiplication precedence\n");
        return 4;
    }
    else if (strcmp(op, "+") == 0 || strcmp(op, "-") ==0)
        return 3;

    else if (strcmp(op, "&") == 0)
        return 2;

    else if (strcmp(op, "|") == 0)
        return 1;
    else
        return -1;
}
int numofpost;

void infix_to_postfix(Token *tokens, Token *postfix) {
    printf("evaluating infix to postfix\n");
    Token stack[MAX_EXPR_LEN];
    int top = -1;
    int i, j;

    for (i = 0, j = 0; i<numtoken; i++) {
        printf("for this is token val :%s \n",tokens[i].value);
        // If the current character is an operand, add it to the postfix expression
        if (tokens[i].type!=4 && (isdigit((int) *(tokens[i].value)) || isalpha((int) *(tokens[i].value)))) {
            printf("type num is %d f\n",tokens[i].type);
            postfix[j++].value = (tokens[i].value);
            printf("this is assigned to postfix j : %s",tokens[i].value);
        }
        // If the current character is an operator, add it to the stack
        else if (strcmp(tokens[i].value, "(") == 0) {
            stack[++top].value = (tokens[i].value);
        }
        else if (strcmp(tokens[i].value, ")") == 0) {
            // Pop operators off the stack and add them to the postfix expression until a matching left parenthesis is encountered
            while (top >= 0 && strcmp(stack[top].value,"(")!=0) {
                postfix[j++].value = stack[top--].value;
            }
            // Discard the left parenthesis
            top--;
        }
        else {
            printf("precedence ölçüm\n");
            printf("top this is: %d",top);
            // Pop operators off the stack and add them to the postfix expression until an operator with lower precedence is encountered
            while (top >= 0 && precedence(stack[top].value) >= precedence(tokens[i].value)) {
                printf("precdence kosulu oldu tamam\n");
                if (strcmp(stack[top].value,",")==0){
                    top--; //pass the comma and reach the function
                    continue;
                }
               
                postfix[j++].value = stack[top--].value;
            }
            // Push the current operator onto the stack
            stack[++top].value = tokens[i].value;
        }
    }

    // Pop any remaining operators off the stack and add them to the postfix expression
    printf("top:::  %d\n",top);
    while (top >= 0) {
        printf(" this is last added to post : %s  \n", stack[top].value);
        postfix[j++].value = stack[top--].value;
    }

    // Add null terminator to the end of the postfix expression
    postfix[j].value = '\0';
    numofpost=j;

    printf("inf to post çıkış\n");
}




// Function to evaluate a postfix expression
int evaluate_postfix(Token *postfix) {
    printf("evaluating\n");
    int stack[MAX_EXPR_LEN];
    int top = -1;
    int i;
    for (i = 0; i<numofpost; i++) {
    // If the current character is an operand, push it onto the stack
    if (isdigit(*postfix[i].value)) {
        int operand = atoi(postfix[i].value);
        stack[++top] = operand;
    }
    else {
        int op1, op2, result;
        char op[32]="";
        int j = 0;
        strcpy(op, postfix[i].value);
        printf("this is op %s",op);
        if (strcmp(op, "xor") == 0) {
            printf("xor catch\n");
            op1 = (stack[top--]);
            printf("op1: %d\n",op1);
            op2 = (stack[top--]);
             printf("op2: %d\n",op2);
            result = op1 ^ op2;
            stack[++top] = result; //integerı stack token valuesu vey bir sekilde atamak lazım
        }
        else if (strcmp(op, "not") == 0) {
            op1 = stack[top--];
            result = ~op1;
            stack[++top] = result;
        }
        else if (strcmp(op, "*") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = op2 * op1;
            stack[++top] = result;
        }
        else if (strcmp(op, "+") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = op2 + op1;
            printf("lastly result %d \n",result);
            stack[++top] = result;
        }
        else if (strcmp(op, "-") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = op2 - op1;
            stack[++top] = result;
        }
        else if (strcmp(op, "&") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result =op2&op1;
            stack[++top] = result;
        }
        else if (strcmp(op, "|") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result=op2|op1;
            stack[++top] = result;
        }
        else if (strcmp(op, "lr") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = (op2 << op1) | (op2 >> (sizeof(op2) * CHAR_BIT - op1));
            stack[++top] = result;
        }
        else if (strcmp(op, "rr") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = (op2 >> op1) | (op2 << (sizeof(op2) * CHAR_BIT - op1));
            stack[++top] = result;
        }
        else if (strcmp(op, "ls") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = (op2 << op1);
            stack[++top] = result;
        }
        else if (strcmp(op, "rs") == 0) {
            op1 = stack[top--];
            op2 = stack[top--];
            result = (op2 >> op1);
            stack[++top] = result;
        }
        else {
            printf("Unknown operator: %s\n", op);
            continue; // Return an error code
        }
    }
   
}
 return stack[top]; 


}



int main(){
    //init_table(Hashtable);
    char line[257]=""; //input line will be stored in here
    printf("Enter>");
    while(fgets(line,sizeof(line),stdin)){

        if (line==NULL ||strcmp(line,"\n")==0){
            //free the memory 
            break; //ctrl+d has entered
        }
        else{
        Token postfixx[257];
        Token tokens[257];
        int numtok=0;
        tokenize(line,tokens,&numtok);
        infix_to_postfix(tokens,postfixx);
        for (int i = 0; i < numtoken; i++) {
        printf("%s\n", postfixx[i].value);
         }
         int res=evaluate_postfix(postfixx);
         printf("RESULT İS: %d\n",res);
       }
        
        }
        
        

    }

