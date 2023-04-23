/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: generator.h - funkce pro generovani IFJcode20
* Autor: Jakub Vano, xvanoj00
         Zuzana Hrklova, xhrklo00
* ***************************************************************/

#ifndef GENERATOR_H
#define GENERATOR_H


#include <stdio.h>
#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"
#include "expressions.h"

/*
* Generovanie vstavanych funkcii
*/
void generate_inputf();
void generate_inputs();
void generate_inputi();
void generate_print();
void generate_int2float();
void generate_float2int();
void generate_len();
void generate_substr();
void generate_ord();
void generate_chr();

/*
* Inicializacia generovania kodu a vstavanych funkcii
*/
void generate_start();
/*
* Generovanie zaciatku main funkcie
*/
void generate_main_start();
/*
* Generovanie konca main funkcie
*/
void generate_main_end();
/*
* Generovanie deklaracie premennej
*/
void generate_var(char *var_name, bool global);

/*
* Generovanie vlozenia hodnoty do premennej
*/
bool generate_assign_value_to_var(int type,char *value, char *var_name, bool global);
/*
* Generovanie vlozenia premennej do premennej
*/
bool generate_assign_var_to_var(char *var_name1,char *var_name2, bool global);

/*
* Priradi k premennej/hodnote jej typ
*/
char *var_define(int type, char *attribute, bool global);
/*
* Generovanie zaciatku funkcie
*/
bool generate_func_start(char *func_name);
/*
* Generovanie konca funkcie
*/
bool generate_func_end();
/*
* Generovanie priradenia argumentu funkcie zo zasobnika
*/
void generate_func_get_argument(char *var_name);
/*
* Generovanie navratovej hodnoty z funkcie
*/
void generate_func_return(Token token, char *var_name, bool global);
/*
* Generovanie vlozeia premennej na zasobnik
*/
void push_param(int type, char * name, bool global);
/*
* Generovanie volania funkcie
*/
bool generate_func_call(char *func_name, int argc);
/*
* Generovanie zaciatku if podmienky
*/
void generate_if_start(bool global, char * bool_value);
/*
* Generovanie else podmienky
*/
void generate_else();
/*
* Generovanie konca if podmienky
*/
void generate_if_end();
/*
* Generovanie zaciatku for cyklu
*/
void generate_for_start(int *for_count, char *var_name, bool global);
/*
* Generovanie konca for cyklu
*/
void generate_for_end();
/*
* Generovanie aritmetickych operacii
*/

bool generate_operators(int type, char * var_dest, Token * var_symb1, Token * var_symb2, bool global_dest,bool global1,bool global2);
static int count = 0;
static int arr_len = 0;
static int * counter_arr;

#endif