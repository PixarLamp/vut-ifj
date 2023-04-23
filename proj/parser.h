/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: parser.h - SA implementovana rekurzivnim sestupem
* Autor: Ondrej Sebesta, xsebes22
* ***************************************************************/

#include "scanner.h"
#include "string.h"
#include "symtable.h"
#include "generator.h"
#include "expressions.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

symtable global_table;
symtable local_table;

char * active_func[99];
char * id_list[99];
int active_counter = 0;
int id_counter = 0;
int arg_counter;
int return_arg_numb;

/*
* Vlozeni vestavenych funkci do TS
*/
void sem_insert_builtin_to_ts();

/*
* Kontrola argumentu funkce (semanticka akce)
*/
void func_arg_check(Token * token, char * func_name );

/*
* Kontrola poctu argumentu funkce (semanticka akce)
*/
void arg_count_check(char * func_name);

/*
* Vicenasobne odradkovani
*/
bool multi_eol( Token * token );


// Funkce implementuji jednotliva pravidla LL gramatiky

/*
* TERM -> string_val
* TERM -> int_val
* TERM -> float64
* TERM -> id
*/
bool NT_term( Token * token );

/*
* EXPRESSION_LIST -> EXPRESSION NEXT_EXPRESSION
*/
bool NT_expression_list( Token * token, bool ret_val, char * id );

/*
* NEXT_EXPRESSION -> , EXPRESSION NEXT_EXPRESSION
*/
bool NT_next_expression( Token * token, char * id );

/*
* RET_VALS -> EXPRESSION_LIST
* RET_VALS -> TERM
* RET_VALS -> ε
*/
bool NT_ret_vals( Token * token );

/*
* OPT_EOL -> eol
* OPT_EOL -> ε
*/
bool NT_opt_eol( Token * token );

/*
* ID_LIST -> , id ID_LIST
* ID_LIST -> ε
*/
bool NT_id_list( Token * token );

/*
* NEXT_ARG -> , TERM NEXT_ARG
* NEXT_ARG -> ε
*/
bool NT_next_arg( Token * token, char * func_name );

/*
* FUNC_ARGS -> TERM NEXT_ARG
* FUNC_ARGS -> ε
*/
bool NT_func_args( Token * token, char * func_name );

/*
* FUNC_CALL -> id ( FUNC_ARGS )
*/
bool NT_func_call( Token * token, char * func_name );

/*
* R_ASSIGN -> FUNC_CALL
* R_ASSIGN -> EXPRESSION_LIST
*/
bool NT_r_assign( Token * token, char * left_id );

/*
* ASSIGN -> id ID_LIST = OPT_EOL R_ASSIGN
*/
bool NT_assign( Token * token );

/*
* ID_DEF -> id := EXPRESSION
*/
bool NT_id_def( Token * token );

/*
* OPT_ID_DEF -> ID_DEF
* OPT_ID_DEF -> ε
*/
bool NT_opt_id_def( Token * token );

/*
* COMMAND_CONTINUE -> := EXPRESSION
* COMMAND_CONTINUE -> ID_LIST = OPT_EOL R_ASSIGN
* COMMAND_CONTINUE -> ( FUNC_ARGS )
*/
bool NT_command_continue( Token * token, char * key );

/*
* COMMANDS -> COMMAND COMMANDS
* COMMANDS -> ε
*/
bool NT_commands( Token * token );

/*
* COMMAND -> if EXPRESSION { eol COMMANDS } else { eol COMMANDS } eol
* COMMAND -> for OPT_ID_DEF ; EXPRESSION ; ID_ASSIGN { eol COMMANDS } eol
* COMMAND -> return RET_VALS eol
* COMMAND -> id COMMAND_CONTINUE eol
*/
bool NT_command( Token * token );

/*
* TYPE -> int
* TYPE -> float64
* TYPE -> string
*/
int NT_type( Token * token );

/*
* NEXT_RET_TYPE -> , TYPE NEXT_RET_TYPE
* NEXT_RET_TYPE -> ε
*/
bool NT_next_ret_type( Token * token );

/*
* RET_TYPES -> ( TYPE NEXT_RET_TYPE )
* RET_TYPES -> ε
*/
bool NT_ret_types( Token * token );

/*
* PARAM -> id TYPE
*/
bool NT_param( Token * token );

/*
* NEXT_PARAM -> , OPT_EOL PARAM NEXT_PARAM
* NEXT_PARAM -> ε
*/
bool NT_next_param( Token * token );

/*
* FUNC_PARAMS -> PARAM NEXT_PARAM
* FUNC_PARAMS -> ε
*/
bool NT_func_params( Token * token );

/*
* FUNC_DEF -> func id ( OPT_EOL FUNC_PARAMS ) RET_TYPES { eol COMMANDS } eol
*/
bool NT_func_def( Token * token );

/*
* NEXT_FUNC_DEFS -> eol FUNC_DEF NEXT_FUNC_DEFS
* NEXT_FUNC_DEFS -> ε
*/
bool NT_next_func_defs( Token * token );

/*
* FUNC_DEFS -> FUNC_DEF NEXT_FUNC_DEFS
*/
bool NT_func_defs( Token * token );

/*
* PROGRAM -> OPT_EOL package main eol FUNC_DEFS OPT_EOL eof
*/
bool NT_program( Token * token );