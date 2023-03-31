#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


//hashtable implementation - needs elaboration
#define TABLE_SIZE 128

typedef struct {
    char* key;
    int value;
} element;

typedef struct {
    element *elements;
    int size;
} table;

table *Hashtable;

int CHAR_BIT = 8;
int error = 0;
int hash_function(char *key);
void init_table(table *table);
void insert(table *table, char *key, int value);
int is_balanced(char *str);
int is_valid_function(char *str);
int check_function(char *line, char *function_name);
int is_valid_operator(char *line, char *operator);

int is_balanced(char *str) { //paranthesis checker for input strings
    int len = strlen(str);
    char stack[len];
    int top = -1;
    for (int i = 0; i < len; i++) {
        if (str[i] == '(') {
            stack[++top] = str[i];
        } else if (str[i] == ')') {
            if (top == -1) {
                return 0;
            } else if (str[i] == ')' && stack[top] == '(') {
                top--;
            } else {
                return 0;
            }
        }
    }
    return (top == -1);
}



int hash_function(char *key){
    int hash = 0;
    int length = strlen(key);
    for(int i = 0; i<length ;i++){
        hash = hash + key[i];
    }
    return hash % TABLE_SIZE;
}

void init_table(table *table) {
    table->elements = (element *)calloc(TABLE_SIZE, sizeof(element));
    table->size = TABLE_SIZE;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->elements[i].key = NULL;
        table->elements[i].value = 0;
    }
}

void insert(table *table, char *key, int value) {
    int index = hash_function(key);
    int i = index;
    while (table->elements[i].key != NULL && strcmp(table->elements[i].key, "") != 0) {
        if (strcmp(table->elements[i].key, key) == 0) {
            table->elements[i].value = value;
            return;
        }
        i = (i + 1) % table->size;
        if (i == index) {
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
            return 0;
        }
    }
    return 0;
}


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
                //in addition directly after the function call, if no left paranthesis is used it is an error. xor(
                
            } else {
                tokens[*num_tokens] = create_token(IDENT, token_str);//string is not a functiopn name

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
    printf("precdence called this is string: %s",op);

    if  (strcmp(op, "=") == 0)
        return 7;

    else if (strcmp(op, "(") == 0 || strcmp(op, ")") == 0)
        return 0;

    else if (strcmp(op, ",") == 0)
        return 1;

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



int allAlpha(char* str) { //checks all the string rather than s string calculator
    int i = 0;
    while (str[i]) {
        if (!isalpha(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

void infix_to_postfix(Token *tokens, Token *postfix) {
    printf("evaluating infix to postfix\n");
    Token stack[MAX_EXPR_LEN];
    int top = -1;
    int i, j;

    for (i = 0, j = 0; i<numtoken; i++) {
        // If the current character is an operand, add it to the postfix expression
        if (tokens[i].type!=4 && (isdigit((int) *(tokens[i].value)) || allAlpha((tokens[i].value)))) {
            printf("type num is %d f\n",tokens[i].type);
            postfix[j++].value = (tokens[i].value);
            printf("this is assigned to postfix : %s",tokens[i].value);
        }
            // If the current character is an operator, add it to the stack
        else if (strcmp(tokens[i].value, "(") == 0) {
            stack[++top].value = (tokens[i].value); // "(" dan sonraki her şeyi ")" görene kadar ekle
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
            if(tokens[i].type==4 && strcmp(tokens[i+1].value, "(") != 0){
                error=1;
            }
            // Pop operators off the stack and add them to the postfix expression until an operator with lower precedence is encountered



            while (top >= 0 && precedence(stack[top].value) >= precedence(tokens[i].value)) {
                printf("precdence kosulu oldu tamam\n");
                if (strcmp(stack[top].value,",")==0){
                    printf("virgül var\n");
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
    while (top >= 0) {
        postfix[j++].value = stack[top--].value;
    }

    // Add null terminator to the end of the postfix expression
    postfix[j].value = '\0';
    numofpost=j;

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
            stack[++top] = operand; //add all operands to stack
        }
        else {
            int op1, op2, result;
            char op[32]=""; //string to store operators,function names or variable names to print out result of that variable
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
                printf("rs gördüm\n");
                op1 = stack[top--];
                op2 = stack[top--];
                result = (op2 >> op1);
                stack[++top] = result;
            }
            else if (isalpha(*postfix[i].value) && strcmp(postfix[i].value,op)==0 ){ //then it is not a function name bec we checked for it before,it is a variable either declared or not
                printf("variable operator: %s\n", op); //sadece bir variable görmesi yetmiyor bu gördüğünün operand ismi ile ynı olması lazım
                result = lookup(Hashtable,op); //bazen mesela a=3%yorum durumunda %yorum kısmı buraya giriyor ve op a iken o sıradaki postfix[i].value %yorum olmasına rağmen a yı printliyor
                stack[++top] = result;

            }
            else if (strcmp(op, ",") == 0){
                //top--;
                continue;

            }

            else {
                printf("Unknown operator.Error!: %s\n", op);
                error=1;
                break;// Return an error code
            }
        } //ELSE IF BITTI

    } // FOR BITTI

    printf("Evaluating for bitti ve top böyle kldı : %d",top);
    if(top!=0){
        printf("Error!\n"); //önemli bir error restrictive i am not sure at all
        error=1;
    }
    return stack[top];


} // CIKIS


char* trim(char* str) {
    char* end;

    // Trim leading whitespace
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) { // All spaces?
        return str;
    }

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

int is_valid_function(char *str){
    int degree = 0;
    int validness = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '(') {
            degree++;
        } else if (str[i] == ')') {
            degree--;
        } else if (str[i] == ',' && degree == 1) {
            validness = 1;
        }
    }

    //paranthess check zatne var alttaki kodu omitliyom.
//    if(degree == 0){
//        return validness; //valid.
//        //we saw a comma and the paranthesis closed.
//    }
    return validness;
}

int check_function(char *line, char *function_name){
    char copyline[strlen(line)];
    strcpy(copyline, line);
    char *func_pos = strstr(copyline, function_name);

    int namesize = strlen(function_name);

    int broken_function = 0;
    while(func_pos != NULL) {
        func_pos = strstr(func_pos, function_name);
        if (func_pos != NULL) {
            func_pos = func_pos + namesize; // second part is the value
            printf("here\n");
            printf("function copyline is ... %s\n",func_pos);
            int valid_function = is_valid_function(func_pos);
            if(valid_function == 1){
                printf("valid\n");
                func_pos = strstr(func_pos, function_name);
                continue;
            }
            else{
                broken_function = 1;
                break;
            }
        }
    }

    if(broken_function){
        return 0;
    }
    else{
        printf("valid function\n");
        return 1;
    }

}


int is_valid_operator(char *line, char *op_name){
    char copyline[strlen(line)];
    strcpy(copyline, line);
    char *op_pos = strstr(copyline, op_name);

    int namesize = 1;

    int broken_op = 0;
    while(op_pos != NULL) {
        op_pos = strstr(op_pos, op_name);
        if (op_pos != NULL) {
            op_pos--; //include the paranthesis
            printf("here\n");
            printf("operator copyline is ... %s\n",op_pos);

            if(*(op_pos) == '(' || *(op_pos+2) == ')'){
                printf("we got sandiwch\n");
                broken_op = 1;
                break;
            }

            else{
                op_pos+=3;
                op_pos = strstr(op_pos, op_name);
                continue;
            }

        }
    }

    if(broken_op){
        return 0; //not valid :(
    }
    else{
        printf("valid operation\n");
        return 1;
    }

}





int main(){

    Hashtable = (table *)malloc(sizeof(table));
    init_table(Hashtable);


    char line[257]=""; //input line will be stored in here
    printf(">");
    while(fgets(line,sizeof(line),stdin) != NULL ){   //CTRL^D inputs are checked by != null... apperently this works on unix sistems


        //blankline inputs
        if(strcmp(line,"\n")==0){
            printf(">");
            continue;
            //for easily terminate in windows doğrusu yukardaki
        }


        //erroneous inputs
        //1)empty paranthesis
        char *paranthesis_pos = strstr(line, "()");
        if(paranthesis_pos != NULL){
            printf("Error!\n");
            printf(">");
            continue;
        }

            //2)lines starting with operations. i.e unary cases like +1+b
        else if(line[0] == '+' ||line[0] == '-' || line[0] == '*' || line[0] == '&'|| line[0] == '|'){
            printf("Error!\n");
            printf(">");
            continue;
        }

        char *commentPos=strchr(line,'%');
        if (commentPos!=NULL){
            *commentPos='\0'; //trim the expression to the % part included \0
        }


            //3)unbalanced paranthesis expressions
        else if(!is_balanced(line)){
            printf("Error!\n");
            printf(">");
            continue;
        }


            //4) broken function calls. We need to evalute the whole strng

            //we will need to do this 5 times...
        else if(!check_function(line,"xor")){
            printf("Error!\n");
            printf(">");
            continue;
        }

        else if(!check_function(line,"ls")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!check_function(line,"rs")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!check_function(line,"rr")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!check_function(line,"lr")){
            printf("Error!\n");
            printf(">");
            continue;
        }

            //function name checking ends.


            //5) we won't allow operators inside paranthesis like 3(+)4

        else if(!is_valid_operator(line, "+")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!is_valid_operator(line, "-")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!is_valid_operator(line, "&")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!is_valid_operator(line, "*")){
            printf("Error!\n");
            printf(">");
            continue;
        }
        else if(!is_valid_operator(line, "|")){
            printf("Error!\n");
            printf(">");
            continue;
        }


        printf("ALL VALIDATIONS DONE\n");
        //assigment check
        char *pos = strchr(line, '=');
        if (pos != NULL) { // if there is an assignment statement
            printf("ASSIGNMENT EVALUATION STARTS\n");
            *pos = '\0'; // replace = with \0
            char *variable = line; // first part is the variable
            char *value = pos + 1; // second part is the value

            variable=trim(variable); //sağ ve soldan boşlukları kırpılmış variable a  =3 çalışır ama a b = 3 çalışmaz
            value=trim(value);
            if (!allAlpha(variable)){
                printf("Given variable is not valid\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("xor",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("ls",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("rs",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("rr",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("lr",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            else if (strcmp("not",variable)==0){
                printf("reserved keyword\n");
                printf("Error!\n");
                printf(">");
                continue;
            }
            printf("variable after trimming and checking: %s",variable);


            // do something with variable and value
            Token postfixx[257];
            Token tokens[257];
            int numtok = 0;
            tokenize(value,tokens,&numtok);
            if(error == 1){
                printf("Error!\n");
                printf(">");
                error = 0; //reset
                continue;
            }

            infix_to_postfix(tokens,postfixx);

            //erroneous tokenization idea
            if(error == 1){
                printf("Error!\n");
                printf(">");
                error = 0; //reset
                continue;
            }


            for (int i = 0; i < numofpost; i++) {
                printf("%s\n", postfixx[i].value);
            }
            int res=evaluate_postfix(postfixx);
            insert(Hashtable,variable,res);

            printf("now our table has %s matched with %d\n", variable, lookup(Hashtable,variable)); //burası line dı variable yaptım çünkü line "    a=4 için boşluklu versiyon "
            printf(">");
            continue;

        }




        else{ //normal expression input exists. no assignment statement
            Token postfixx[257];
            Token tokens[257];
            int numtok = 0;

            tokenize(line,tokens,&numtok);
            if(error == 1){
                printf("Error!\n");
                printf(">");
                error = 0; //reset
                continue;
            }

            infix_to_postfix(tokens,postfixx);
            if(error == 1){
                printf("Error!\n");
                printf(">");
                error = 0; //reset
                continue;
            }
            
            for (int i = 0; i < numofpost; i++) {
                printf("%s\n", postfixx[i].value);
            }
            int res=evaluate_postfix(postfixx);
            if(error == 1){
                printf("Error!\n");
                printf(">");
                error = 0; //reset
                continue;
            }

            printf("RESULT İS: %d\n",res);
            printf(">");

            continue;
        }

    }

    printf("end of program");

}

/**
* isvalid function modifiye edilip 3(+)4 tarzı işlemlerden korumaya alınacak.
*/
