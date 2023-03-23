#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef enum {
    NONE, //end token of array to understand conclusion,after "%" also can be none
    NUM, //all numbers 
    OP, //+,-,&,|,*
    IDENT, //identifiers,initally zero
    FUNC_CALL, //for all functions in the format function(arg1,arg2)
    LPAR,
    RPAR,
    COMMA,
    ASSIGN

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
        else if (curr_char == '+' || curr_char == '-' || curr_char == '*'   ||
                   curr_char == '(' || curr_char == ')'|| curr_char == '&'|| curr_char == '|') {
                    
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
        else {
            i++;
            curr_char = input[i];
        }
    }
    for (int i = 0; i < *num_tokens; i++) {
    //Token token = tokens[i];

    
    //printf("Token %d: type=%d, value=%s \n", i, tokens[i].type,tokens[i].value);
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


//PARSER
enum Parse_Tokens {
    TOKEN_NOT, TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_AND, TOKEN_OR, TOKEN_XOR,
    TOKEN_LS, TOKEN_RS, TOKEN_LR, TOKEN_RR,
    TOKEN_NUM, TOKEN_VAR, TOKEN_COMMA, TOKEN_LP, TOKEN_RP, TOK_EOF
};

/**
 * struct tokendata part
 * @return
 */

//FUNCTIONS
int parse_program();
int parse_expression();
int parse_term();
int parse_factor();
int parse_multiplicand();
int parse_xor();
int parse_not();
int parse_shift_rotation();
int parse_left_shift();
int parse_right_shift();
int parse_left_rotation();
int parse_right_rotation();
int parse_prop();
int parse_number();
int parse_variable();

void get_next_token();


//the parse_expression function example from slides c-ified



/* a converter
 * initialize current_token_type
 * implement get_next_token
 */


int convert(Token tok[]){
     Token token=tok[current_index];
     printf("token value to be converted: %s     \n ",tok[current_index].value);
     printf("this is token type:  %d this is vl: %s     \n",token.type,token.value);

    if(strcmp(token.value,"(") == 0) { // "LP"
        current_token_type = TOKEN_LP;
    }
    else if(strcmp(token.value,")") == 0) { // "RP""
        current_token_type = TOKEN_RP;
    }
    else if(strcmp(token.value,",") == 0) { // ","
        current_token_type = TOKEN_COMMA;

    }
    else if(strcmp(token.value,"not") == 0) { // "tilda"
        current_token_type = TOKEN_NOT;
    }

    else if(strcmp(token.value,"+")==0) {
        printf("arti bulundu convert edildi");
        current_token_type = TOKEN_PLUS;
    }

    else if(strcmp(token.value,"-")==0) {
        current_token_type = TOKEN_MINUS;
    }

    else if(strcmp(token.value,"*")==0) {
        current_token_type = TOKEN_MULT;
    }
    else if(strcmp(token.value,"&")==0) {
        current_token_type = TOKEN_AND;
    }

    else if(strcmp(token.value,"|")==0) {
        current_token_type = TOKEN_OR;
    }

    else if(strcmp(token.value,"xor")== 0) {
        current_token_type = TOKEN_XOR;
    }

    else if(strcmp(token.value,"ls")==0) {
        current_token_type = TOKEN_LS;
    }

    else if(strcmp(token.value,"rs")==0) {
        current_token_type = TOKEN_RS;
    }
    else if(strcmp(token.value,"lr")==0) {
        current_token_type = TOKEN_LR;
    }
    else if(strcmp(token.value,"rr")==0) {
        current_token_type = TOKEN_RR;
    }

        //NUM AND VAR TYPES
        //needs regex...
        //i only make nums
    else {
        current_token_type = TOKEN_NUM;
    }
    printf("converting completed this is current type : %d     \n",current_token_type);

    return current_token_type; //this is the new type of token it is a number like 1,2,3,4

}


void get_next_token() {
    if (tokens[current_index].value == NULL || tokens[current_index].type == NONE) {
        return;

    } else {
    current_token_type = convert(tokens);
    //printf("Getting next token...\n this is new token type number %d\n and its value: %s", current_token_type,
     //      tokens[current_index].value);
    current_index++;
        return;
}
}

//bitwise or
int parse_program(){
    int expression = parse_expression();
    while(current_token_type == TOKEN_OR) { //least importance
        int temp= current_token_type;
        get_next_token();
        if(temp == TOKEN_OR){
            expression = expression | parse_expression();
            printf("9&5"
                   "OR OPERATİON RESULT İS Result is %d\n",expression);
        }
    }
    return expression;
}


//bitwise and
int  parse_expression(){
    int term = parse_term();
    while (current_token_type == TOKEN_AND) {
        int temp= current_token_type;
        get_next_token(); //advance method in psget_next_token(); //advance method in ps
        if (temp == TOKEN_AND) {
            term = term & parse_term();
        }
    }
    return term;
}

//addition and subtraction
int  parse_term(){
    int factor = parse_factor();
    while (current_token_type == TOKEN_PLUS || current_token_type == TOKEN_MINUS) {
        int temp= current_token_type;
        get_next_token(); //advance method in ps
        if (temp == TOKEN_PLUS) {
            printf("\ntoken plus operation adding up %s + %s \n",tokens[current_index-1].value, tokens[current_index+1].value);
            factor += parse_factor();
        } else { //minus operation
            factor -= parse_factor();
        }
    }
    return factor;
}

//multiplication
int  parse_factor(){
    int multiplicand = parse_multiplicand();
    while (current_token_type == TOKEN_MULT) {
        get_next_token();
        multiplicand *= parse_multiplicand();
    }
    return multiplicand;
}

//function calls ls rs lr rr not xor
int parse_multiplicand( ){
    int prop = parse_prop();
    while(current_token_type == TOKEN_LS ||current_token_type == TOKEN_RS ||current_token_type == TOKEN_LR  ||current_token_type == TOKEN_RR  ||current_token_type == TOKEN_XOR  ||current_token_type == TOKEN_NOT  ||current_token_type == TOKEN_NUM  ||current_token_type == TOKEN_VAR){
        get_next_token();
        if(current_token_type == TOKEN_XOR){
            return parse_xor();
        }
        else if(current_token_type == TOKEN_NOT){
            return parse_not();
        }
        else if(current_token_type == TOKEN_NUM){
            return parse_number();
        }
        else if (current_token_type == TOKEN_VAR) {
            return parse_variable();
        }
        else{ //rotations
            return parse_shift_rotation();
        }
    }
    return prop;
}

int parse_xor(){
    int prop;
    int prop1;
    int prop2;
    get_next_token(); // (
    get_next_token(); // first prop
    prop1 = parse_prop();
    get_next_token(); // ,
    printf("now we have the comma %d",current_token_type);
    get_next_token(); //second prop
    prop2 = parse_prop();
    get_next_token(); // )
    printf("XORING PROP1 %d AND PROP2  %d BECOMES %d", prop1, prop2, prop1^prop2);
    prop = prop1 ^ prop2;
    return prop;
}



int parse_not(){
    int prop;
    get_next_token(); // (
    get_next_token(); // first prop
    prop = parse_prop();
    get_next_token(); // )
    printf("NEGATING PROP %d BECOMES %d", prop, ~prop);
    prop = ~prop;
    return prop;
}


int parse_shift_rotation(){
    while(current_token_type == TOKEN_LS ||current_token_type == TOKEN_RS ||current_token_type == TOKEN_LR ||current_token_type == TOKEN_RR){
        if(current_token_type == TOKEN_LS){
            return parse_left_shift();
        }
        else if(current_token_type == TOKEN_RS){
            return parse_right_shift();
        }
        else if(current_token_type == TOKEN_LR){
            return parse_left_rotation();
        }
        else if(current_token_type == TOKEN_RR){
            return parse_right_rotation();
        }
        //error
    }
}

int parse_left_shift(){
    int prop;
    get_next_token(); // (
    get_next_token(); // first prop
    prop = parse_prop();
    get_next_token(); // ,
    get_next_token(); //shift num
    int num = parse_number();
    get_next_token(); // )
    prop = prop << num;
    return prop;
}

int parse_right_shift(){
    int prop;
    get_next_token(); // (
    get_next_token(); // first prop
    prop = parse_prop();
    get_next_token(); // ,
    get_next_token(); // shift num
    int num = parse_number();
    get_next_token(); // )
    prop = prop >> num;
    return prop;
}

//rotation differs from shift in that it preserves fallen bits by gluing them back on to the other end.

int parse_left_rotation(){
    int prop;
    get_next_token(); // (
    get_next_token(); // first prop
    prop = parse_prop();
    get_next_token(); // ,
    get_next_token(); // rotation num
    int num = parse_number();
    get_next_token(); // )
    prop = (prop << num) | (prop >> (8 - num)); //why did we use 8?
    return prop;
}

int parse_right_rotation(){
    int prop;
    get_next_token(); // (
    get_next_token(); // first prop
    prop = parse_prop();
    get_next_token(); // ,
    get_next_token(); // rotation num
    int num = parse_number();
    get_next_token(); // )
    prop = (prop >> num) | (prop << (8 - num));  //why did we use 8? do i need to hardcode another number
    return prop;
}

int parse_prop(){
    int prop;
    if(current_token_type == TOKEN_NUM){
        prop = parse_number();
    }
    else if(current_token_type == TOKEN_VAR){
        printf("idnt\n");
        prop = parse_variable();
    }
    return prop;
}
int parse_number(){
    int num;
    
    num=atoi(tokens[current_index-1].value);
    printf("    number expected,  before :%s,now: '%s' then : '%s' \n",tokens[current_index-1].value,tokens[current_index].value,tokens[current_index+1].value);
    printf("NUMBER OF TOKENS %d, CURRENT INDEX %d", num_tokens,current_index);
    if(current_index != 0) {
        get_next_token();
    }
    return num;
}

int parse_variable(){
    int var;
    var=0;
    printf("encountered with a variable (identifier) this is : &s \n",tokens[current_index-1].value);
    if(current_index != 0) {
        get_next_token();
    }
    return var;
}


int main(){

    char line[257]="";
    //input line will be stored in here
    printf("Enter>");

    while(fgets(line,sizeof(line),stdin)){
        printf("Enter>");

        if (line==NULL || line=="12345"){
            //free the memory
            break; //ctrl+d has entered
        }
        else{
            int numtok = 0;
            tokenize(line,tokens,&numtok);
            for(int i = 0; i<257; i++){
                printf("%s", tokens[i].value);
            }
            int result = parse_program(); //not sure of denotation
            printf("        RESULT: %d  \n", result);
            //get_next_token();
            result=0;
            for(int i =  0; i<257;i++){ //Numtok may be
                tokens[i].type = NONE;
            }
            current_index = 0;
            continue;
        }


    }//return 0;
}


