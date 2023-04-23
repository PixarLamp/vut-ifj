/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: parser.c - SA implementovana rekurzivnim sestupem
* Autor: Ondrej Sebesta, xsebes22
* ***************************************************************/

#include "parser.h"

int counter = 0;
char * ts_key = "";

void sem_insert_builtin_to_ts()
{
    // func inputs() (string,int)
    insert_symbol( &global_table, "inputs");
    item_data * inputs = search_item( &global_table, "inputs", "inputs" );
    if ( inputs == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! inputs() \n" ); exit( INTERN_ERR ); }
    inputs->inputs_no = 0;
    inputs->outputs_no = 2;
    inputs->out_params[0] = T_STRING_LITERAL;
    inputs->out_params[1] = T_INTEGER;

    // func inputi() (int,int)
    insert_symbol( &global_table, "inputi");
    item_data * inputi = search_item( &global_table, "inputi", "inputi" );
    if ( inputi == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! inputi() \n" ); exit( INTERN_ERR ); }
    inputi->inputs_no = 0;
    inputi->outputs_no = 2;
    inputi->out_params[0] = T_INTEGER;
    inputi->out_params[1] = T_INTEGER;

    // func inputf() (float64,int)
    insert_symbol( &global_table, "inputf");
    item_data * inputf = search_item( &global_table, "inputf", "inputf" );
    if ( inputf == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! inputf() \n" ); exit( INTERN_ERR ); }
    inputf->inputs_no = 0;
    inputf->outputs_no = 2;
    inputf->out_params[0] = T_FLOATING_POINT_DOUBLE;
    inputf->out_params[1] = T_INTEGER;

    // func print ( term1 , term2 , ..., termx )
    insert_symbol( &global_table, "print");
    item_data * print = search_item( &global_table, "print", "print" );
    if ( print == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! print() \n" ); exit( INTERN_ERR ); }
    print->outputs_no = 0;

    //func int2float(i int) (float64) 
    insert_symbol( &global_table, "int2float");
    item_data * int2float = search_item( &global_table, "int2float", "int2float" );
    if ( int2float == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! int2float() \n" ); exit( INTERN_ERR ); }
    int2float->inputs_no = 1;
    int2float->outputs_no = 1;
    int2float->in_params[0] = T_INTEGER;
    int2float->out_params[0] = T_FLOATING_POINT_DOUBLE;

    // func float2int(f float64) (int)
    insert_symbol( &global_table, "float2int");
    item_data * float2int = search_item( &global_table, "float2int", "float2int" );
    if ( float2int == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! float2int() \n" ); exit( INTERN_ERR ); }
    float2int->inputs_no = 1;
    float2int->outputs_no = 1;
    float2int->in_params[0] = T_FLOATING_POINT_DOUBLE;
    float2int->out_params[0] = T_INTEGER;

    // func len(𝑠 string) (int)
    insert_symbol( &global_table, "len");
    item_data * len = search_item( &global_table, "len", "len" );
    if ( len == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! len() \n" ); exit( INTERN_ERR ); }
    len->inputs_no = 1;
    len->outputs_no = 1;
    len->in_params[0] = T_STRING_LITERAL;
    len->out_params[0] = T_INTEGER;

    // func substr(s string, i int, n int) (string, int)
    insert_symbol( &global_table, "substr");
    item_data * substr = search_item( &global_table, "substr", "substr" );
    if ( substr == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! substr() \n" ); exit( INTERN_ERR ); }
    substr->inputs_no = 3;
    substr->outputs_no = 2;
    substr->in_params[0] = T_STRING_LITERAL;
    substr->in_params[1] = T_INTEGER;
    substr->in_params[2] = T_INTEGER;
    substr->out_params[0] = T_STRING_LITERAL;
    substr->out_params[1] = T_INTEGER;
    
    // func ord(s string, i int) (int, int)
    insert_symbol( &global_table, "ord");
    item_data * ord = search_item( &global_table, "ord", "ord" );
    if ( ord == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! ord() \n" ); exit( INTERN_ERR ); }
    ord->inputs_no = 2;
    ord->outputs_no = 2;
    ord->in_params[0] = T_STRING_LITERAL;
    ord->in_params[1] = T_INTEGER;
    ord->out_params[0] = T_INTEGER;
    ord->out_params[1] = T_INTEGER;

    // func chr(i int) (string, int)
    insert_symbol( &global_table, "chr");
    item_data * chr = search_item( &global_table, "chr", "chr" );
    if ( chr == NULL ) { fprintf( stderr, "Chyba [INTERN]> Chyba vlozeni do TS! chr() \n" ); exit( INTERN_ERR ); }
    chr->inputs_no = 1;
    chr->outputs_no = 2;
    chr->in_params[0] = T_INTEGER;
    chr->out_params[0] = T_STRING_LITERAL;
    chr->out_params[1] = T_INTEGER;

}


// Lexikalni analyza - tisk tokenu
void lex_debug( int token_type, char * token_attribute )
{
    fprintf( stderr, "[ %d|%s ]\n" , token_type, token_attribute );
}

// EOLy
bool multi_eol( Token * token )
{
    while ( token->token_type == T_END_OF_LINE ) {
        get_next_token( token );
    }

    put_token_back( token );
    return true;

}

// $ TERM -> string_val
// $ TERM -> int_val
// $ TERM -> float64
// $ TERM -> id
bool NT_term( Token * token )
{

    switch ( token->token_type ) {
        case T_STRING_LITERAL:
        case T_INTEGER:
        case T_FLOATING_POINT_DOUBLE:
        case T_FLOATING_POINT_EXPONENT_DOUBLE:
        case T_EXPONENT_DOUBLE:
        case T_IDENTIFIER:
            return true;
            break;
        default:
            return false;
    }

}

// EXPRESSION_LIST -> EXPRESSION NEXT_EXPRESSION
bool NT_expression_list( Token * token, bool ret_val, char * id )
{
    return_arg_numb = 0;
    fprintf(stderr, "AA%d", token->token_type );
    if ( token->token_type == T_IDENTIFIER || token->token_type == T_INTEGER || 
         token->token_type == T_EXPONENT_DOUBLE || token->token_type == T_FLOATING_POINT_DOUBLE ||
         token->token_type == T_FLOATING_POINT_EXPONENT_DOUBLE ) {
        
        char ids[99];
        if (ret_val) {
            precedence_processing( token, &local_table, "return");
            strcpy(ids,"return");
        } else {
           
            get_next_token(token);           
            precedence_processing( token, &local_table, id);
            memcpy(ids,id,sizeof(&id));

        }

        return NT_next_expression( token, ids );
        
    }

    return false;

}


// EXPRESSION_LIST -> , EXPRESSION NEXT_EXPRESSION
bool NT_next_expression( Token * token, char * id )
{

    if ( token->token_type == T_COMA ) {

        get_next_token( token );
        return_arg_numb++;
        precedence_processing(token,&local_table,id);
        

        return NT_next_expression( token, id );

    }

    put_token_back( token );
    return true;

}

// $ RET_VALS -> EXPRESSION_LIST
// $ RET_VALS -> TERM
// $ RET_VALS -> ε
bool NT_ret_vals( Token * token )
{
    
    if ( NT_expression_list( token , true, "" ) ) {

        put_token_back( token );
        return true;

    } else if ( NT_term( token ) ) {

        return true;

    } 

    put_token_back( token );
    return true;

}

// $ OPT_EOL -> eol
// $ OPT_EOL -> ε
bool NT_opt_eol( Token * token )
{

    if ( token->token_type == T_END_OF_LINE ) {

        return multi_eol( token );

    }
        
    put_token_back( token );
    return true;

}

// $ ID_LIST -> , id ID_LIST
// $ ID_LIST -> ε
bool NT_id_list( Token * token )
{
    
    if ( token->token_type == T_COMA ) {

        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );

        if ( token->token_type == T_IDENTIFIER ) {
            id_list[id_counter] = token->token_attribute;
            id_counter++;
            get_next_token( token );
            lex_debug( token->token_type, token->token_attribute );
            return NT_id_list( token );
            
        } else { return false; }

    }

    put_token_back( token );
    return true;

}

// $ NEXT_ARG -> , TERM NEXT_ARG
// $ NEXT_ARG -> ε
bool NT_next_arg( Token * token, char * func_name )
{

    if ( token->token_type == T_COMA ) {

        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );

        if ( NT_term( token ) ) {
            arg_counter++;

            if(strcmp(func_name,"print"))
                func_arg_check(token,func_name);        
                               
            push_param(token->token_type,token->token_attribute,false);

            get_next_token( token );
            lex_debug( token->token_type, token->token_attribute );            
            
            
             
            return NT_next_arg( token, func_name );

        } else { return false; }

    }
    
    put_token_back( token );
    return true;

}

void arg_count_check(char * func_name){
    item_data * func = search_item(&global_table, func_name, func_name );    
    if(func == NULL){
        exit(SEM_NAME_ERR);
    }
    if(func->inputs_no != arg_counter-1){        
        exit(SEM_PARAM_ERR);
    }
}
void func_arg_check(Token * token, char * func_name  ){
    item_data * func = search_item(&global_table, func_name, func_name );
    if(func == NULL){
        exit(SEM_NAME_ERR);
    }
    if (token->token_type == T_IDENTIFIER)
    {                           
        item_data * ID = search_item(&local_table, active_func[active_counter-1], token->token_attribute );    
        if(ID != NULL){
            if(func->in_params[arg_counter-1] != ID->t_data){
                exit(SEM_PARAM_ERR);
            }   
        }else{
            exit(SEM_PARAM_ERR);
        }
        
    }else{                
        if(func->in_params[arg_counter-1] != token->token_type){             
            exit(SEM_PARAM_ERR);
        }
    }
}

// $ FUNC_ARGS -> TERM NEXT_ARG
// $ FUNC_ARGS -> ε
bool NT_func_args( Token * token, char * func_name )
{
    arg_counter++;  

    
    if ( NT_term( token ) ) {
        
        if(strcmp(func_name,"print"))
            func_arg_check(token,func_name);

        push_param(token->token_type,token->token_attribute,false);
        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );
        return NT_next_arg( token, func_name );

    } 

    put_token_back( token );
    return true;

}

// FUNC_CALL -> id ( FUNC_ARGS )
bool NT_func_call( Token * token, char * func_name )
{
    arg_counter = 0;    
    if ( token->token_type == T_IDENTIFIER ) {

        Token * tmp = (Token *) malloc(sizeof(Token));
        if ( tmp == NULL ) {
            fprintf( stderr, "Chyba [INTERN]> Chyba alokace paměti! \n" );
            exit( INTERN_ERR );
        }
        
        tmp->token_attribute = token->token_attribute;
        tmp->token_type = token->token_type;

        get_next_token( token );

        if ( token->token_type == T_LEFT_PARENTHESIS ) {

            get_next_token( token );
            lex_debug( token->token_type, token->token_attribute );
            
            if ( NT_func_args( token, func_name ) ) {

                arg_counter++;
                if(strcmp(func_name,"print"))                 
                    arg_count_check(func_name); 

                get_next_token( token );
                lex_debug( token->token_type, token->token_attribute );
                
                if ( token->token_type == T_RIGHT_PARENTHESIS ) {                                        

                    return true;

                } else { put_token_back( token ); return false; }

            } else { put_token_back( token ); return false; }

        } else {
            
            for (int i = 0; i < strlen(token->token_attribute); i++)
            {
                ungetc(token->token_attribute[i],stdin);
            }

            token->token_attribute = tmp->token_attribute;
            token->token_type = tmp->token_type;
            return false;
        }

    } 

    put_token_back( token );
    return false;

}

// R_ASSIGN -> FUNC_CALL
// R_ASSIGN -> EXPRESSION_LIST
bool NT_r_assign( Token * token, char * left_id)
{
    char func_name[99];
                    
    memcpy(func_name,token->token_attribute,sizeof(&token->token_attribute));
    if ( NT_func_call( token, func_name ) ) {
        if(strcmp(func_name,"print"))                         
            arg_count_check(func_name);
        
        generate_func_call(func_name,arg_counter-1);
        for (int i = 0; i < id_counter; i++)
        {
            generate_func_get_argument(id_list[i]) ;
        }
        get_next_token( token );
        return true;

    } else if ( NT_expression_list( token, false, left_id) ) {      
        put_token_back( token );
        return true;

    }else
    {
        exit(SYNTAX_ERR);
    }

    return false;

}

// ASSIGN -> id ID_LIST = OPT_EOL R_ASSIGN
bool NT_assign( Token * token )
{
    
    if ( token->token_type == T_IDENTIFIER ) {

        get_next_token( token );
        id_counter = 0;
        id_list[id_counter] = token->token_attribute;
        id_counter++;
        
        if ( NT_id_list( token ) ) {
            
            get_next_token( token );

            if ( token->token_type == T_ASSIGNMENT ) {

                get_next_token( token );

                if ( NT_opt_eol( token ) ) {

                    char left_id[99];                    
                    memcpy(left_id,token->token_attribute,sizeof(&token->token_attribute));
                                        
                    get_next_token( token );
                    
                    return NT_r_assign( token, left_id);

                } else { return false; }

            } else { return false; }

        } else { return false; }

    }

    return false;

}

// ID_DEF -> id := EXPRESSION
bool NT_id_def( Token * token )
{

    bool id_def = false;

    if ( token->token_type == T_IDENTIFIER ) {

        // SEM Byla uz promenna definovana?
        item_data * exists = search_item(&local_table, token->token_attribute, token->token_attribute );
        if ( exists != NULL ) { fprintf( stderr, "Chyba [SEM] > Pokus o redefinici promennes! \n" ); exit( SEM_NAME_ERR ); }

        // Vloz identifikator do TS
        insert_symbol( &global_table, token->token_attribute );
        

        char * key = token->token_attribute;

        get_next_token( token );

        if ( token->token_type == T_DEFINITION ) {

            get_next_token( token );

            if ( token->token_type == T_IDENTIFIER ) {

                get_next_token( token );

                if ( token->token_type == T_LEFT_PARENTHESIS ) {
                } else {

                    precedence_processing( token, &local_table, key);
                    

                }

            } else {
                
            }

        }

    }

    return id_def;

}

// OPT_ID_DEF -> ID_DEF
// OPT_ID_DEF -> ε
bool NT_opt_id_def( Token * token )
{

    if ( NT_id_def( token ) ) {

        return true;

    }

    put_token_back( token );
    return true;

}

// COMMAND_CONTINUE -> := EXPRESSION
// COMMAND_CONTINUE -> ID_LIST = OPT_EOL R_ASSIGN
// COMMAND_CONTINUE -> ( FUNC_ARGS )
bool NT_command_continue( Token * token, char * key )
{
    
    bool command_continue = false;
    char func_name[99]; 
    
    memcpy(func_name,token->token_attribute,sizeof(&token->token_attribute)); 
       
    get_next_token( token );

    // COMMAND_CONTINUE -> := EXPRESSION
    if ( token->token_type == T_DEFINITION ) {
        


        int index = hash_function(active_func[active_counter-1]);


        item_data * exists = search_item(&local_table, active_func[active_counter-1], key );
        if ( exists != NULL ) { fprintf( stderr, "Chyba [SEM] > Pokus o redefinici promennes! \n" ); exit( SEM_NAME_ERR ); }
        
        table_item *new_item = (table_item*)malloc(sizeof(table_item));
         initialize_string(&new_item->key);

         new_item->key = realloc(new_item->key,sizeof(key));
         
         
         memcpy(new_item->key,key,sizeof(&key));

        if (local_table[index] == NULL) {
            local_table[index] = new_item;
            new_item->next = NULL;

        } else {
            
            new_item->next = local_table[index];
            local_table[index] = new_item;                        
        }

        get_next_token( token );

        if ( token->token_type == T_IDENTIFIER ) {

            Token * tmp = (Token *) malloc(sizeof(Token));
            if ( tmp == NULL ) {
                fprintf( stderr, "Chyba [INTERN]> Chyba alokace pameti! \n" );
                exit( INTERN_ERR );
            }

            get_next_token( tmp );
            for (int i = 0; i < strlen(tmp->token_attribute); i++)
            {
                ungetc(tmp->token_attribute[i],stdin);
            }

            if ( tmp->token_type == T_LEFT_PARENTHESIS ) {
                
                put_token_back(tmp);
                token = tmp;
                return NT_command_continue(token, key );
                
            } else {
            
                precedence_processing( token, &local_table, key);
                return true;

            }

        } else {
                     
            precedence_processing( token, &local_table, key);
            
            return true;

        }

    // COMMAND_CONTINUE -> ( FUNC_ARGS )
    } else if ( token->token_type == T_LEFT_PARENTHESIS ) {
        
        get_next_token( token );
        
        if ( NT_func_args( token, func_name ) ) {
            arg_counter++;
            if(strcmp(func_name,"print"))                 
                arg_count_check(func_name);            
            get_next_token( token );
            
            if ( token->token_type == T_RIGHT_PARENTHESIS ) {
                generate_func_call(func_name,arg_counter-1);
                for (int i = 0; i < id_counter; i++)
                {
                    generate_func_get_argument(id_list[i]) ;
                }
                get_next_token( token );

                return true;

            }

        }

    // COMMAND_CONTINUE -> ID_LIST = OPT_EOL R_ASSIGN
    } else if ( NT_id_list( token ) ) {
        id_list[id_counter] = func_name;
        id_counter++;
        get_next_token( token );

        if ( token->token_type == T_ASSIGNMENT ) {

            get_next_token( token );

            if ( NT_opt_eol( token ) ) {

                get_next_token( token );                       
                return NT_r_assign( token, func_name );

            }

        }

    } 

    return false;

}

// COMMANDS -> COMMAND COMMANDS
// COMMANDS -> ε
bool NT_commands( Token * token )
{
     
    arg_counter = 0;

    if ( NT_command( token ) ) {
        
        get_next_token( token );
        return NT_commands( token );

    }
    
    put_token_back( token );
    return true;

}

// COMMAND -> if EXPRESSION { eol COMMANDS } else { eol COMMANDS } eol
// COMMAND -> for OPT_ID_DEF ; EXPRESSION ; ID_ASSIGN { eol COMMANDS } eol
// COMMAND -> return RET_VALS eol
// COMMAND -> id COMMAND_CONTINUE eol
bool NT_command( Token * token )
{

    if ( token->token_type == T_KEYWORD ) {

        // COMMAND -> if EXPRESSION { eol COMMANDS } else { eol COMMANDS }
        if ( !strcmp( token->token_attribute, "if" ) ) {

            get_next_token( token );
            char * ret_value = precedence_processing( token, &local_table, "" );
            generate_if_start(false, ret_value);

            if ( token->token_type == T_LEFT_CURLY_BRACKET ) {

                get_next_token( token );

                if ( token->token_type == T_END_OF_LINE ) {
                    multi_eol( token );

                    get_next_token( token );

                    if ( NT_commands( token ) ) {
                    
                        get_next_token( token );

                        if ( token->token_type == T_RIGHT_CURLY_BRACKET ) {

                            get_next_token( token );

                            if ( token->token_type == T_KEYWORD ) {
                                if ( !strcmp( token->token_attribute, "else" ) ) {
                                    generate_else();
                                    get_next_token( token );

                                    if ( token->token_type == T_LEFT_CURLY_BRACKET ) {

                                        get_next_token( token );

                                        if ( token->token_type == T_END_OF_LINE ) {

                                            multi_eol( token );
                                            get_next_token( token );

                                            if ( NT_commands( token ) ) {

                                                get_next_token( token );

                                                if ( token->token_type == T_RIGHT_CURLY_BRACKET ) {

                                                    get_next_token( token );

                                                    if ( token->token_type == T_END_OF_LINE ) {
                                                        generate_if_end();
                                                        multi_eol( token );
                                                        return true;

                                                    }
                                                    
                                                }

                                            }

                                        }

                                    }

                                }
                            }

                        }

                    }

                }

            }

        // COMMAND -> for OPT_ID_DEF ; EXPRESSION ; ID_ASSIGN { eol COMMANDS }
        } else if ( !strcmp( token->token_attribute, "for" ) ) {

            get_next_token( token );

            if ( NT_opt_id_def( token ) ) {

                get_next_token( token );
                
                if ( token->token_type == T_SEMICOLON ) {
                    
                    get_next_token( token );

                    precedence_processing( token, &local_table, "" );
                    
                    if ( token->token_type == T_SEMICOLON ) {

                        get_next_token( token );

                        if ( NT_assign( token ) ) {

                            get_next_token( token );

                            if ( token->token_type == T_LEFT_CURLY_BRACKET ) {

                                get_next_token( token );

                                if ( token->token_type == T_END_OF_LINE ) {

                                    multi_eol( token );
                                    get_next_token( token );

                                    if ( NT_commands( token ) ) {

                                        get_next_token( token );

                                        if ( token->token_type == T_RIGHT_CURLY_BRACKET ) {

                                            get_next_token( token );

                                            if ( token->token_type == T_END_OF_LINE ) {

                                                return multi_eol( token );

                                            }

                                        }

                                    }

                                }

                            }

                        }

                    }

                }

            }

        // COMMAND -> return RET_VALS
        } else if ( !strcmp( token->token_attribute, "return" ) ) {
            
            get_next_token( token );

            if ( NT_ret_vals( token ) ) {

                get_next_token( token );

                if ( token->token_type == T_END_OF_LINE ) {

                    return multi_eol( token );

                }

            }

        }

    // COMMAND -> id COMMAND_CONTINUE
    } else if ( token->token_type == T_IDENTIFIER ) {

        char * key = token->token_attribute;

        id_counter = 0;

        if ( NT_command_continue( token, key ) ) {
            
            if ( token->token_type == T_END_OF_LINE ) {

                return multi_eol( token );

            }

        }
                

    } 

    return false;

}

// TYPE -> int
// TYPE -> float64
// TYPE -> string
int NT_type( Token * token )
{

    if ( token->token_type == T_KEYWORD ) {

        if ( !strcmp( token->token_attribute, "int" ) ) {

            return T_INTEGER;

        } else if ( !strcmp( token->token_attribute, "float64" ) ) {

            return T_FLOATING_POINT_DOUBLE;

        } else if ( !strcmp( token->token_attribute, "string" ) ) {

            return T_STRING_LITERAL;

        }

    }

    return 0;

}

// NEXT_RET_TYPE -> , TYPE NEXT_RET_TYPE
// NEXT_RET_TYPE -> ε
bool NT_next_ret_type( Token * token )
{

    if ( token->token_type == T_COMA ) {

        get_next_token( token );

        int tmp = NT_type( token );
        if ( tmp ) {                                        
            
            item_data * item = search_item( &global_table, ts_key ,ts_key);
            if ( item != NULL ) {
                item->out_params[item->outputs_no] = tmp;
                char name[99];                
                snprintf(name, sizeof(name), "%s%d", "return",item->outputs_no);                         
                generate_var(name,false);                 
                item->outputs_no++;                
            }

            get_next_token( token );
            return NT_next_ret_type( token );

        } else { return false; }

    }

    put_token_back( token );
    return true;

}

// RET_TYPES -> ( TYPE NEXT_RET_TYPE )
// RET_TYPES -> ε
bool NT_ret_types( Token * token )
{

    if ( token->token_type == T_LEFT_PARENTHESIS ) {

        get_next_token( token );

        int tmp = NT_type( token );
        if ( tmp ) {

            item_data * item = search_item(&global_table, ts_key, ts_key );
            if ( item != NULL ) {
                item->out_params[item->outputs_no] = tmp;                    
                char name[99];                
                snprintf(name, sizeof(name), "%s%d", "return",item->outputs_no);                         
                generate_var(name,false);                 
                item->outputs_no++;               
            }
            
            get_next_token( token );
            
            if ( NT_next_ret_type( token ) ) {

                get_next_token( token );

                if ( token->token_type == T_RIGHT_PARENTHESIS ) {
                    return true;

                } else { return false; }

            } else { return false; }

        } else { return false; }

    } 
    
    put_token_back( token );
    return true;

}

// $ PARAM -> id TYPE
bool NT_param( Token * token )
{

    if ( token->token_type == T_IDENTIFIER ) {

        get_next_token( token );

        // SEM naplneni tabulky symbolu
        int tmp = NT_type( token );
        if ( tmp ) {

            item_data * item = search_item(&global_table, ts_key, ts_key );
            if ( item != NULL ) {
                item->in_params[item->inputs_no] = tmp;
                item->inputs_no++;
            }
            return true;

        }

    }

    return false;

}

// $ NEXT_PARAM -> , OPT_EOL PARAM NEXT_PARAM
// $ NEXT_PARAM -> ε
bool NT_next_param( Token * token )
{

    if ( token->token_type == T_COMA ) {

        get_next_token( token );

        if ( NT_opt_eol( token ) ) {

            get_next_token( token );

            if ( NT_param( token ) ) {

                get_next_token( token );
                return NT_next_param( token );

            } else { return false; }

        } else { return false; }

    }

    put_token_back( token );
    return true;

}

// FUNC_PARAMS -> PARAM NEXT_PARAM
// FUNC_PARAMS -> ε
bool NT_func_params( Token * token )
{

    if ( NT_param( token ) ) {

        get_next_token( token );
        return NT_next_param( token );

    } 

    put_token_back( token );
    return true;

}

// FUNC_DEF -> func id ( OPT_EOL FUNC_PARAMS ) RET_TYPES { eol COMMANDS }
bool NT_func_def( Token * token )
{
    bool func_def = false;

    if ( !strcmp( token->token_attribute, "func" ) ) {

        get_next_token( token );

        if ( token->token_type == T_IDENTIFIER ) {

            renew_string( &ts_key, token->token_attribute );

            if ( !strcmp(ts_key, "main" ) ) {

                generate_main_start();

            } else {

                // GEN pocatek definice funkce
                generate_func_start( token->token_attribute );

            }
            
            insert_symbol( &global_table, token->token_attribute );
            active_func[active_counter] = token->token_attribute;
            active_counter++;

            get_next_token( token );

            if ( token->token_type == T_LEFT_PARENTHESIS ) {
                
                get_next_token( token );

                if ( NT_opt_eol( token ) ) {
                    
                    get_next_token( token );

                    if ( NT_func_params( token ) ) {

                        get_next_token( token );

                        if ( token->token_type == T_RIGHT_PARENTHESIS ) {

                            get_next_token( token ); // RET_TYPES

                            if ( NT_ret_types( token ) ) {
                                
                                get_next_token( token ); // [ { ]

                                if ( token->token_type == T_LEFT_CURLY_BRACKET ) {
                                    
                                    get_next_token( token ); // [ eol ]

                                    if ( token->token_type == T_END_OF_LINE ) {

                                        multi_eol( token );
                                        get_next_token( token ); // NT_COMMANDS

                                        if ( NT_commands( token ) ) {

                                            get_next_token( token ); // [ } ]

                                            if ( token->token_type == T_RIGHT_CURLY_BRACKET ) {
                                                
                                                if ( !strcmp( ts_key, "main" ) ) {
                                                    
                                                    generate_main_end();
                                                }else{
                                                    item_data * func = search_item(&global_table, ts_key, ts_key);
                                                    char retname[99];
                                                    
                                                    for (int i = func->outputs_no-1 ; i >= 0; i--)
                                                    {
                                                        snprintf(retname,sizeof(retname),"%s%d","return",i);
                                                        push_param(T_IDENTIFIER,retname,false);
                                                    }    
                                                    generate_func_end();
                                                    active_counter--; 
                                                }
       
                                                func_def = true;

                                            }


                                        }


                                    }


                                }

                            }


                        }

                    }

                }


            }


        }

    }

    return func_def;

}

// NEXT_FUNC_DEFS -> eol FUNC_DEF NEXT_FUNC_DEFS
// NEXT_FUNC_DEFS -> ε
bool NT_next_func_defs( Token * token )
{

    if ( token->token_type == T_END_OF_LINE ) {

        multi_eol( token );
        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );

        if ( NT_func_def( token ) ) {

            get_next_token( token );
            lex_debug( token->token_type, token->token_attribute );
            return NT_next_func_defs( token );

        } else if ( token->token_type == T_END_OF_FILE ) {

            put_token_back( token );
            return true;

        }

    }

    put_token_back( token );
    return true;

}

// FUNC_DEFS -> FUNC_DEF NEXT_FUNC_DEFS
bool NT_func_defs( Token * token )
{

    if ( NT_func_def( token ) ) {

        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );
        return NT_next_func_defs( token );

    }

    return false;

}

// PROGRAM -> OPT_EOL package main eol FUNC_DEFS OPT_EOL eof
bool NT_program( Token * token )
{

    if ( NT_opt_eol( token ) ) {

        get_next_token( token );
        lex_debug( token->token_type, token->token_attribute );

        if ( !strcmp( token->token_attribute, "package" ) ) {
        
            get_next_token( token );
            lex_debug( token->token_type, token->token_attribute );
            
            if ( !strcmp( token->token_attribute, "main" ) ) {

                get_next_token( token );
                lex_debug( token->token_type, token->token_attribute );

                if ( token->token_type == T_END_OF_LINE ) {

                    multi_eol( token );
                    get_next_token( token );
                    lex_debug( token->token_type, token->token_attribute );

                    if ( NT_func_defs( token ) ) {

                        get_next_token( token );
                        lex_debug( token->token_type, token->token_attribute );
                        
                        if ( token->token_type == T_END_OF_FILE ) {

                            return true;

                        } else { return false; }

                    } else { return false; }

                } else { return false; }

            } else { return false; }

        } else { return false; }

    }

    return false;

}

int main( int argc, char *argv[] )
{    
    generate_start();
    Token * token = (Token *) malloc(sizeof(Token));

    if ( token == NULL ) {
        fprintf( stderr, "Chyba [INTERN]> Chyba alokace paměti! \n" );
        exit( INTERN_ERR );
    }

    token->returned = false;

    initialize_symtable( &global_table );
    initialize_symtable( &local_table );
    initialize_string( &ts_key );
    sem_insert_builtin_to_ts();

    get_next_token( token );

    if ( NT_program( token ) ) {

        // Syntakticka analyza skonci uspechem, kontrola existence funkce main
        item_data * main = search_item(&global_table, "main", "main" );
        if ( main == NULL ) { fprintf( stderr, "Chyba [SEM] > Neexistujici funkce main! \n" ); exit( SEM_NAME_ERR ); }
        if ( main->inputs_no != 0 ) { fprintf( stderr, "Chyba [SEM] > Funkce main neprijima zadne parametry! \n" ); exit( SEM_PARAM_ERR ); }
        if ( main->outputs_no != 0 ) { fprintf( stderr, "Chyba [SEM] > Funkce main nevraci zadnou hodnotu! \n" ); exit( SEM_PARAM_ERR ); }

        // Uspesne ukonceni behu prekladace
        exit( 0 );

    } else {        
        exit( SYNTAX_ERR );

    }

    return 0;
    
}