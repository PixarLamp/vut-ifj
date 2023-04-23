/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: generator.c - funkce pro generovani IFJcode20
* Autor: Jakub Vano, xvanoj00
         Zuzana Hrklova, xhrklo00
* ***************************************************************/

#include "generator.h"
#include "scanner.h"
#include "string.h"

//vstavane funkcie
void generate_inputf()
{
    float numb = 0.0;
    fprintf(stdout, "LABEL $inputf\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "READ LF@return float\n"
                    "JUMPIFEQ $err LF@return nil@nil\n"  
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n"
                    "LABEL $err\n"
                    "PUSHS float@%a\n"
                    "PUSHS int@1\n"
                    "POPFRAME\n"
                    "RETURN\n",numb);
}

void generate_inputs()
{
    fprintf(stdout, "LABEL $inputs\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "READ LF@return string\n"
                    "JUMPIFEQ $err LF@return nil@nil\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_inputi()
{
    fprintf(stdout, "LABEL $inputi\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "READ LF@return int\n"
                    "JUMPIFEQ $err LF@return nil@nil\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_print()
{
    fprintf(stdout, "LABEL $print\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@temp\n"
                    "DEFVAR LF@iterator\n"
                    "POPS LF@iterator\n"
                    "LABEL $cycle\n"
                    "JUMPIFEQ $cycle_end LF@iterator int@0\n"
                    "POPS LF@temp\n"
                    "WRITE LF@temp\n"
                    "SUB LF@iterator LF@iterator int@1\n"
                    "JUMP $cycle\n"
                    "LABEL $cycle_end\n"                    
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_int2float()
{
    fprintf(stdout, "LABEL $int2float\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "DEFVAR LF@int\n"
                    "POPS LF@int\n"
                    "INT2FLOAT LF@return LF@int\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_float2int()
{
    fprintf(stdout, "LABEL $float2int\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "DEFVAR LF@float\n"
                    "POPS LF@float\n"
                    "FLOAT2INT LF@return LF@float\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_len()
{
    fprintf(stdout, "LABEL $len\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "DEFVAR LF@string\n"
                    "POPS LF@string\n"
                    "STRLEN LF@return LF@string\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_substr()
{
    fprintf(stdout, "LABEL $substr\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@string\n"
                    "DEFVAR LF@index\n"
                    "DEFVAR LF@length\n"
                    "POPS LF@length\n"
                    "POPS LF@index\n"
                    "POPS LF@string\n"
                    "DEFVAR LF@check\n"
                    "DEFVAR LF@str_length\n"
                    "STRLEN LF@str_length LF@string\n"
                    "SUB LF@str_length LF@str_length int@1\n"
                    "GT LF@check LF@index LF@str_length\n"
                    "JUMPIFEQ $err LF@check bool@true\n"
                    "LT LF@check LF@index int@0\n"
                    "JUMPIFEQ $err LF@check bool@true\n"
                    "LT LF@check LF@length int@0\n"
                    "JUMPIFEQ $err LF@check bool@true\n"
                    "DEFVAR LF@return\n"
                    "TYPE LF@return LF@return\n"
                    "DEFVAR LF@tmp\n"
                    "LABEL $cycle2\n"
                    "GT LF@check LF@index LF@str_length\n"
                    "JUMPIFEQ $cycle_end2 LF@check bool@true\n"
                    "JUMPIFEQ $cycle_end2 LF@length int@0\n"
                    "STRI2INT LF@tmp LF@string LF@index\n"
                    "INT2CHAR LF@tmp LF@tmp\n"
                    "CONCAT LF@return LF@return LF@tmp\n"
                    "ADD LF@index LF@index int@1\n"
                    "SUB LF@length LF@length int@1\n"
                    "JUMP $cycle2\n"
                    "LABEL $cycle_end2\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_ord()
{
    fprintf(stdout, "LABEL $ord\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@return\n"
                    "DEFVAR LF@tmp\n"
                    "DEFVAR LF@check\n"
                    "DEFVAR LF@index\n"
                    "POPS LF@index\n"
                    "DEFVAR LF@string\n"
                    "POPS LF@string\n"
                    "STRLEN LF@tmp LF@string\n"
                    "SUB LF@tmp LF@tmp int@1\n"
                    "GT LF@chcek LF@index LF@tmp\n"
                    "JUMPIFEQ $err LF@check bool@true\n"
                    "LT LF@check LF@index int@0\n"
                    "JUMPIFEQ $err LF@check bool@true\n"
                    "STRI2INT LF@return LF@string LF@index\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}

void generate_chr()
{
    fprintf(stdout, "LABEL $chr\n"
                    "PUSHFRAME\n"
                    "DEFVAR LF@tmp\n"
                    "DEFVAR LF@return\n"
                    "POPS LF@return\n"
                    "LT LF@tmp LF@return int@0\n"
                    "JUMPIFEQ $err LF@tmp bool@true\n"
                    "GT LF@tmp LF@return int@255\n"
                    "JUMPIFEQ $err LF@tmp bool@true\n"
                    "INT2CHAR LF@return LF@$return\n"
                    "PUSHS LF@return\n"
                    "POPFRAME\n"
                    "RETURN\n");
}



void generate_start()
{
    fprintf(stdout, ".IFJcode20\n"
                    "JUMP $$main\n");
    
    generate_inputs();
    generate_inputi();
    generate_inputf();
    generate_print();
    generate_int2float();
    generate_float2int();
    generate_len();
    generate_substr();
    generate_ord();
    generate_chr();
}

void generate_main_start()
{
    fprintf(stdout, "LABEL $$main\n"
                    "CREATEFRAME\n"
                    "PUSHFRAME\n");
}

void generate_main_end()
{
    fprintf(stdout, "POPFRAME\n");
}

void generate_var(char *var_name, bool global)
{
    //vytvorenie premennej ak je globalna/lokalna
    if(global){
        fprintf(stdout, "DEFVAR GF@%s\n"
                        "MOVE GF@%s nil@nil\n", var_name, var_name);
    }
    else{
        fprintf(stdout, "DEFVAR LF@%s\n"
                        "MOVE LF@%s nil@nil\n", var_name, var_name);
    }
}

bool generate_assign_value_to_var(int type,char *value, char *var_name, bool global)
{
    char *param;
    char *var;
    
    param = var_define(T_IDENTIFIER, var_name, global);
    var = var_define(type, value, global);

    if (param == NULL || var == NULL){
        return false;
    }

    fprintf(stdout, "MOVE %s %s\n", param, var);
    //uvolnenie alokovanej pamete
    free(param);
    free(var);
    return true;
}
bool generate_assign_var_to_var(char *var_name1, char *var_name2, bool global)
{
    char *param;
    char *var;
    
    param = var_define(T_IDENTIFIER, var_name2, global);
    var = var_define(T_IDENTIFIER, var_name1, global);

    if (param == NULL || var == NULL){
        return false;
    }

    fprintf(stdout, "MOVE %s %s\n", param, var);
    //uvolnenie alokovanej pamete
    free(param);
    free(var);
    return true;
}

char *var_define(int type, char *attribute, bool global){
    char *tmp;
    
    char pom[99];
    strcpy(pom,attribute);
    
    //podla typu ulozi na zaciatok datovy typ/ramec
    switch(type){
        case T_IDENTIFIER:
            tmp = malloc(sizeof(pom) + sizeof(char)*5);
            if(!tmp){
                return NULL;
            }
            if (strcmp(attribute, "$return") == 0)                
                strcpy(tmp, "TF@");
            else if(!global){                
                strcpy(tmp, "LF@");
            }
            else{                
                strcpy(tmp, "GF@");
            }
                                    
            strcat(tmp, pom);
            return tmp;
        case T_INTEGER:            
            tmp = malloc(sizeof(pom) + sizeof(char)*5);
            if(!tmp){
                return NULL;
            }
            strcpy(tmp, "int@");            
            strcat(tmp,pom);            
            return tmp;
        case T_STRING_LITERAL:
            tmp = malloc(strlen(attribute) + 8);
            if(!tmp){
                return NULL;
            }
            strcpy(tmp, "string@");
            strcpy(tmp + 7, attribute);
            return tmp;
        case T_FLOATING_POINT_DOUBLE:
            tmp = malloc(strlen(attribute) + 7);
            if(!tmp){
                return NULL;
            }
            strcpy(tmp, "float@");
            float normal_form = atof(attribute);
            char name[99];
            snprintf(name, sizeof(name), "%a",normal_form);           
            strcpy(tmp + 6, name);
            return tmp;
        case BOOL: 
            tmp = malloc(sizeof(pom) + sizeof(char)*5);
            if(!tmp){
                return NULL;
            }
            strcpy(tmp, "bool@");            
            strcat(tmp,pom);            
            return tmp;   
        default:
            return NULL;

    }
}

bool generate_func_start(char *func_name)
{
    fprintf(stdout, "LABEL $%s\n"
                    "PUSHFRAME\n", func_name);
    return true;
}

bool generate_func_end()
{
    fprintf(stdout, "POPFRAME\n"
                    "RETURN\n");
    return true;
}

void generate_func_get_argument(char *var_name)
{
    fprintf(stdout, "POPS LF@%s\n", var_name);
}

void generate_func_return(Token token, char *var_name, bool global)
{
    char *var;
    var = var_define(token.token_type, token.token_attribute, global);
    fprintf(stdout, "MOVE LF@return %s\n", var);
    free(var);
}

void push_param(int type, char * name, bool global){
    char *var;
    var = var_define(type, name, global);
    fprintf(stdout, "PUSHS %s\n", var);
    free(var);
}

bool generate_func_call(char *func_name, int argc)
{
    fprintf(stdout, "PUSHS int@%d\n"
                    "CREATEFRAME\n"
                    "CALL $%s\n", argc, func_name);
    return true;
}

void generate_if_start(bool global, char * bool_value)
{           
    char *var = var_define(T_IDENTIFIER, bool_value, global);
    fprintf(stdout, "JUMPIFEQ $if_else%d %s bool@false\n", count, var);
    free(var);    
    if(count == 0){
        counter_arr = malloc(sizeof(int)*10); 
    }else{
        if(sizeof(counter_arr) < sizeof(int)*(arr_len+1)){
            counter_arr = realloc(counter_arr,sizeof(counter_arr)+sizeof(int)*10);
        }        
    }
    
    counter_arr[arr_len] = count++;
    arr_len++;
}

void generate_else()
{    

    fprintf(stdout, "JUMP $if_end%d\n"
                    "LABEL $if_else%d\n", counter_arr[arr_len], counter_arr[arr_len]);
    
}

void generate_if_end()
{
    arr_len--;
    fprintf(stdout, "LABEL $if_end%d\n", counter_arr[arr_len]);
}

void generate_for_start(int *for_count, char *var_name, bool global)
{
    char *var;
    static int counter = 0;
    var = var_define(T_IDENTIFIER, var_name, global);
    fprintf(stdout, "LABEL $for%d\n"
                    "JUMPIFEQ $for_end%d %s bool@false\n", counter, counter, var);
    *for_count = counter++;
    //uvolnenie alokovanej pamete
    free(var);
}

void generate_for_end(int for_count)
{
    fprintf(stdout, "JUMP $for%d\n"
                    "LABEL $for_end%d\n", for_count, for_count);
}

bool generate_operators(int type, char * var_dest, Token * var_symb1, Token * var_symb2, bool global_dest,bool global1,bool global2)
{
    char *var;
    char *symb1;
    char *symb2;
    var = var_define(T_IDENTIFIER, var_dest, global_dest);
    symb1 = var_define(var_symb1->token_type, var_symb1->token_attribute, global1);
    symb2 = var_define(var_symb2->token_type, var_symb2->token_attribute, global2);

    //podla typu vygeneruje danu aritmeticku operaciu
    switch (type)
    {
    case T_ADDITION:
        fprintf(stdout, "ADD %s %s %s\n", var, symb1, symb2);
        break;
    case T_SUBTRACTION:
        fprintf(stdout, "SUB %s %s %s\n", var, symb1, symb2);
        break;
    case T_MULTIPLICATION:
        fprintf(stdout, "MUL %s %s %s\n", var, symb1, symb2);
        break;
    case T_DIVISION:
        if(var_symb1->token_type == T_INTEGER && var_symb2->token_type == T_INTEGER)
        {
            fprintf(stdout, "IDIV %s %s %s\n", var, symb1, symb2);
        }
        else {
            fprintf(stdout, "DIV %s %s %s\n", var, symb1, symb2);
        }
        break;
    case T_LESS_THAN:
        fprintf(stdout, "LT %s %s %s\n", var, symb1, symb2);
        break;
    case T_LESS_OR_EQUAL_TO:
        fprintf(stdout, "DEFVAR LF@tmp\n"
                        "LT %s %s %s\n"
                        "EQ LF@tmp %s %s\n"
                        "OR %s LF@tmp %s\n", var, symb1, symb2, symb1, symb2, var, var);
        break;
    case T_GREATER_THAN:
        fprintf(stdout, "GT %s %s %s\n", var, symb1, symb2);
        break;
    case T_GREATER_OR_EQUAL_TO:
        fprintf(stdout, "DEFVAR LF@tmp\n"
                        "GT %s %s %s\n"
                        "EQ LF@tmp %s %s\n"
                        "OR %s LF@tmp %s\n", var, symb1, symb2, symb1, symb2, var, var);
        break;
    case T_EQUAL_TO:
        fprintf(stdout, "EQ %s %s %s\n", var, symb1, symb2);
        break;
    case T_NOT_EQUAL_TO:
        fprintf(stdout, "EQ %s %s %s\n"
                        "NOT %s %s\n", var, symb1, symb2, var, var);
        break;
    default:
        return false;
    }
    return true;
}