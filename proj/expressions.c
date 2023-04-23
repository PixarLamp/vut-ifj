/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 
*
* Soubor: precedencni_analyza.c - zpracovani vyrazu - parser
* Autor: Jiří Václavič, xvacla31
* ***************************************************************/

#include "expressions.h"

int LF_counter = 0;
bool func_defi;

struct Stack* firstPush() 
{

    struct Stack (*stack) = (struct Stack*)malloc(sizeof(struct Stack));

    if ( stack == NULL ) {
        fprintf( stderr, "Chyba [INTERN]> Chyba alokace paměti! \n" );
        exit( INTERN_ERR );
    }

    stack->array[0] = (struct Item*)malloc(sizeof(struct Item));
    initialize_string( &stack->array[0]->item );

    if ( stack->array[0] == NULL ) {
        fprintf( stderr, "Chyba [INTERN]> Chyba alokace paměti! \n" );
        exit( INTERN_ERR );
    }

    stack->capacity = 1;
    stack->top = -1;     
    stack->array[++stack->top]->type = STOP;
    strcpy(stack->array[stack->top]->item,"STOP");
    return stack;

} 
  

  
// Zasobnik je prazdny, kdyz je vrchol roven -1
int Empty(struct Stack* stack) 
{ 
    return stack->top == -1; 
} 

bool isInteger(double val)
{
    int truncated = (int)val;
    return (val == truncated);
} 

// Funkce prida polozku na zasobnik, inkrementuje jeho velikost
void push(struct Stack* stack,char * item_name_param, int item_type) 
{ 

    char item_name[99];
    memcpy(item_name,item_name_param,sizeof(&item_name_param));        

    stack->array[stack->top+1] = (struct Item*)malloc(sizeof(struct Item));
    initialize_string( &stack->array[stack->top+1]->item );
    if(stack->array[stack->top+1] == NULL){
        fprintf( stderr, "Alokace pameti selhala!\n");
        exit(INTERN_ERR);
    }
    
    strcpy(stack->array[stack->top+1]->item,item_name);
    stack->array[stack->top+1]->type = item_type;
    
    stack->top +=1;       
    fprintf(stderr,"%s pushed to stack\n", item_name); 
} 
  
// Funkce ostrani polozku ze zasobniku a dekrementuje jeho velikost
int pop(struct Stack* stack) 
{ 
    if (!Empty(stack))
     
        stack->top--;        
        if(stack->top > 0){            
            return stack->array[stack->top]->type; 
        }else{            
            return STOP;
        }

} 

// Precedencni tabulka
char precedence_table[7][7] = {
/*                +-   /x  ==  (   )  var  $     */
/*  +-   */      {'>','<','>','<','>','<','>'},
/*  /x   */      {'>','>','>','<','>','<','>'},
/*  ==   */      {'<','<','X','<','>','<','>'},
/*  (    */      {'<','<','<','<','=','<','X'}, //k
/*  )    */      {'>','>','>','X','>','X','>'},
/*  var  */      {'>','>','>','X','>','X','>'}, //k
/*  $    */      {'<','<','<','<','X','<','K'}
};

int get_index( int token_type )
{
 int index;

 switch (token_type) {
    case T_ADDITION:
    case T_SUBTRACTION:
        index = 0;
        break;
    case T_MULTIPLICATION:
    case T_DIVISION:
        index = 1;
        break;
    case T_EQUAL_TO:
    case T_NOT_EQUAL_TO:
    case T_GREATER_THAN:
    case T_LESS_THAN:
    case T_GREATER_OR_EQUAL_TO:
    case T_LESS_OR_EQUAL_TO:
        index = 2;
        break;
    case T_LEFT_PARENTHESIS:
        index = 3;
        break;
    case T_RIGHT_PARENTHESIS:
        index = 4;
        break;
    case T_IDENTIFIER:   
    case T_INTEGER:
    case T_FLOATING_POINT_DOUBLE:
    case T_FLOATING_POINT_EXPONENT_DOUBLE:
    case T_STRING_LITERAL:
        index = 5;
        break;
    case T_END_OF_LINE:
    case T_END_OF_FILE:
    case T_LEFT_CURLY_BRACKET:
    case T_SEMICOLON:
    case T_COMA:
    case STOP:
        index = 6;
        break;       
    default:
      
        fprintf( stderr, "Chyba ve funkci get_index!\n");
        exit(SYNTAX_ERR);
        break;
 }
 return index;
}

// Ziskani indexu do precedencni tabulky
void get_table_index(Token* token, struct Stack* stack, int* input_index, int* stack_index,int* left_shift){

    *input_index = get_index(token->token_type);     
    *stack_index = get_index(stack->array[stack->top]->type);
   
    if(*stack_index == 5 && stack->array[stack->top-1]->type != LEFT_OP){
        *left_shift +=1;
        *stack_index = get_index(stack->array[stack->top-1]->type);
    }
}

// Hledani v precedencni tabulce za pomoci drive nalezenych indexu
char table_value_search(int input_index, int stack_index)
{
    fprintf(stderr,"\ninput-%i stack-%i \n", input_index,stack_index);
    char value = precedence_table[stack_index][input_index];
    if(value == 'X'){
        fprintf( stderr, "Chyba ve funkci table_value_search\n");
        exit(SYNTAX_ERR);
    }
    fprintf(stderr,"VALUEE: %c\n",value);
    return value;
}

void sub_expr_process( struct Stack* stack, symtable * table, char * key_param,Token* token )
{
    char key[99];
    strcpy(key,key_param);
    for(int i = stack->top; i > 0; i--)
    {
        if(stack->array[i]->type == LEFT_OP)
        {      
            int sub_expr_len = stack->top - i; // zjisteni delky podvyrazu 
            
            char name[99];
            char value[99];

            snprintf(name, sizeof(name), "%%%d", LF_counter);
          
            switch (sub_expr_len)
            {
                //Redukce vyrazu na stacku obsahujici jeden prvek mezi zavorky <> napr. < var > -> E
                case 1:                    
                    
                    if(stack->array[i+1]->type >= 20 && stack->array[i+1]->type <=24 || stack->array[i+1]->type==T_IDENTIFIER){                        

                        stack->array[i]->type=stack->array[i+1]->type;
                        if(stack->array[i]->type == T_IDENTIFIER){
                            item_data * variable;
                            variable = search_item( table, active_func[active_counter-1],stack->array[i+1]->item );
                            
                            if ( variable == NULL && strcmp(key,"")) {                                
                                exit(SYNTAX_ERR);
                            }else if(variable != NULL){
                                stack->array[i]->id_type = variable->t_data;
                            }
                        }
                        stack->array[i]->item = stack->array[i+1]->item;

                    }else{
                        exit(INTERN_ERR);
                    }
                    
                    pop(stack); // pop <                    
                    break;

                //Redukce vyrazu na stacku obsahujici tri prvky mezi zavorky <> napr. < E + E > -> E
                case 3:                                     
                    expr_eval(stack,i);
                    break;
                default:
                    fprintf( stderr, "Chyba ve funkci sub_expr_process\n");
                    exit(SYNTAX_ERR);
                    break;
            }
            break;
        }
    }
}

bool check_set_int(int type, int* stack_type){
    if(type == T_INTEGER){
        *stack_type = T_INTEGER;
        return true;
    }else{
        return false;
    }

}
bool check_set_float(int type, int* stack_type){
    if(type == T_FLOATING_POINT_DOUBLE || type == T_FLOATING_POINT_EXPONENT_DOUBLE){
        *stack_type = T_FLOATING_POINT_DOUBLE;
        return true;
    }else{                
        return false;
    }

}

void expr_eval(struct Stack* stack,int i)
{    
    struct Item* operand1 = stack->array[i+1];
    struct Item* operand2 = stack->array[i+3];
    struct Item* operator = stack->array[i+2];
    Token * token1 = (Token *) malloc(sizeof(Token));
    Token * token2 = (Token *) malloc(sizeof(Token));
    Token * token_dest = (Token *) malloc(sizeof(Token));
    
    initialize_string( &token1->token_attribute );
    initialize_string( &token2->token_attribute ); 
    initialize_string( &token_dest->token_attribute );    
       
    token1->token_type = operand1->type;
    token2->token_type = operand2->type;
    
    if(
        (operand1->type != operand2->type) && !(
            (operand1->type == T_FLOATING_POINT_EXPONENT_DOUBLE && operand2->type == T_FLOATING_POINT_DOUBLE) ||
            (operand2->type == T_FLOATING_POINT_EXPONENT_DOUBLE && operand1->type == T_FLOATING_POINT_DOUBLE) || 
            (operand1->type == T_IDENTIFIER && operand1->id_type == operand2->type) ||
            (operand2->type == T_IDENTIFIER && operand2->id_type == operand1->type)) || 
        (operand1->type == T_IDENTIFIER && operand2->type == T_IDENTIFIER && operand1->id_type != operand2->id_type)
        ){        
        fprintf( stderr, "Chyba ve funkci expr_eval\n");
        exit(SEM_TYPE_ERR);
        
    }

    if (operand1->type != T_FLOATING_POINT_EXPONENT_DOUBLE && operand2->type == T_FLOATING_POINT_DOUBLE)
    {
        fprintf(stderr,"Operand: %i %i OP2: %i",operand1->type,operand1->id_type,operand2->type);
    }
    
    char name[99];    

    snprintf(name, sizeof(name), "%%%d", LF_counter);    

    memcpy(token1->token_attribute,operand1->item,sizeof(&operand1->item));            
    memcpy(token2->token_attribute,operand2->item,sizeof(&operand2->item));

    generate_var(name,false);
     
    LF_counter++;
    generate_operators(operator->type,name,token1,token2,false,operand1->global,operand2->global);
    stack->array[i]->type= T_IDENTIFIER;
    stack->array[i]->id_type = operand2->type;
    memcpy(stack->array[i]->item,name,sizeof(&stack->array[i]->item));
    
    pop(stack);
    pop(stack);
    pop(stack);

}

void remove_left_OP(struct Stack* stack){
    for(int i = stack->top;i > 0;i--){
        if(stack->array[i]->type == LEFT_OP && stack->array[i+1]->type == T_LEFT_PARENTHESIS){
            free(stack->array[i]);
            free(stack->array[i+1]);
            for(int i2 = i; i < stack->top; i++){
                stack->array[i2] = stack->array[i2+2];
            }
            stack->top -= 2;
            break;
        }
    }
}

bool token_check(Token* token){
    if( (token->token_type == T_END_OF_FILE) || 
        (token->token_type == T_END_OF_LINE) ||
        (token->token_type == T_LEFT_CURLY_BRACKET) ||
        (token->token_type == T_SEMICOLON) ||
        (token->token_type == T_COMA)
    ){
        return false;
    }
    else{
        return true;
    }
}

// Hlavni funkce precedencni analyzy
char * precedence_processing( Token * token, symtable * table, char * key) 
{ 

    //fprintf( stderr, "[[%s]]", token->token_attribute );
    if(!token_check(token)){
        exit(SYNTAX_ERR);
    }

    // nacteni zarazky na zasobnik
    struct Stack* stack = firstPush();
    int left_shift = 0;
    //fprintf(stderr,"\nNA VSTUPUX JE %i\n",token->token_type);

    while(true)
    {

        fprintf(stderr,"Stack: ");
        for(int i = 0;i <= stack->top;i++)
        {
            fprintf(stderr,"%s ",stack->array[i]->item);
        }
        
        int input_index, stack_index;

        get_table_index(token, stack, &input_index, &stack_index,&left_shift);        
        char operation = table_value_search(input_index,stack_index);
        
        if(operation == '<')
        {
            push(stack,"<", LEFT_OP);
            struct Item* pom =  stack->array[stack->top-left_shift];
            stack->array[stack->top-left_shift] = stack->array[stack->top];
            stack->array[stack->top] = pom;
            
            if(token->token_type == T_IDENTIFIER){

                item_data * variable;
                
                variable = search_item( table, active_func[active_counter-1],token->token_attribute );                
                
                if ( variable != NULL){
                  
                    char value[99];
                     
                    switch (variable->t_data)
                    {
                    case T_INTEGER:                        
                        push(stack,token->token_attribute,T_IDENTIFIER);
                        break;
                    case T_FLOATING_POINT_DOUBLE:
                    case T_FLOATING_POINT_EXPONENT_DOUBLE:                                                                      
                        push(stack,token->token_attribute,T_IDENTIFIER);                    
                        break;
                    case T_STRING_LITERAL:
                        push(stack,token->token_attribute,T_IDENTIFIER);                                                                                                         
                        break; 
                         
                    default:                                                                        
                      
                        exit(SYNTAX_ERR);
                        break;
                    }

                }else if(strcmp(key,"")){
                    
                    exit(SYNTAX_ERR);
                }else{
                    push(stack,token->token_attribute,T_UNDEFINED);
                }                     
            }else{
                push(stack,token->token_attribute,token->token_type);
            }
    
            for(int i = 0;i <= stack->top;i++)
            {
                fprintf(stderr,"%s ",stack->array[i]->item);
            }
            fprintf(stderr,"\n");
            fprintf(stderr, "[[%s]]", token->token_attribute);
            if(token_check(token)){
               get_next_token( token );
               fprintf(stderr, "[[%s]]", token->token_attribute);
               fprintf(stderr, "[[%s]]", token->token_attribute);
           } 
                                  
        }
        else if(operation == '>')
        {                       
            sub_expr_process(stack,table,key,token); // vyhodnoceni podvyrazu
        }
        else if(operation == '=')
        {
            remove_left_OP(stack);
            if(token_check(token)){
               get_next_token( token );
               fprintf(stderr, "[[%s]]", token->token_attribute);
           } 
        }
        else if(operation == 'K'){                        

            item_data * variable = search_item( table, active_func[active_counter-1], key );
            
            if(!strcmp(key,"return")){ 
                item_data * func = search_item( &global_table, active_func[active_counter-1], active_func[active_counter-1] );
                char name[99];
                
                if( func != NULL){
                    
                    snprintf(name, sizeof(name), "%s%d", "return",return_arg_numb);
                                                                               
                    generate_assign_value_to_var(stack->array[1]->type,stack->array[1]->item,name,false);                    
                }                
                
                return NULL;
            }

            if(variable == NULL) {
                //fprintf(stderr,"Zas tu?\n ----- %s ----\n",key);
            }
            
            if ( variable == NULL && strcmp(key,"")) {                
                exit(SYNTAX_ERR);
            }else if(variable != NULL){ 
                                              
                if (stack->array[1]->type != T_IDENTIFIER){                                       
                    variable->t_data = stack->array[1]->type ;
                }else{
                    variable->t_data = stack->array[1]->id_type ;                    
                }                
                if(!variable->already_defined){
                    generate_var(key,false);  
                }

                variable->already_defined = true;                
                generate_assign_value_to_var(stack->array[1]->type,stack->array[1]->item,key,variable->global); 
                
               
            } else {
                return stack->array[1]->item;
            } 
            


            break;            
        }
        left_shift = 0;
    }    
    pop(stack);
    pop(stack);
    return NULL; 
}