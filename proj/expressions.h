/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 
*
* Soubor: precedencni_analyza.c - zpracovani vyrazu - parser
* Autor: Jiří Václavič, xvacla31
* ***************************************************************/
#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "generator.h"

extern char * active_func[];
extern int active_counter;
extern symtable global_table;
extern int return_arg_numb;

struct Item {

    char * item; // nazev
    int type; // typ
    bool global;
    int id_type;
};

struct Stack {

    int top; 
    unsigned capacity; 
    struct Item *array[99];

};

/*
*   Funkce je volána z parseru, zpracovává výraz v cyklu a podvýrazy vkládá na zásobník,
    kde je následně redukuje na jediný. Během redukce výrazu se volají patřičné funkce z generátoru kódu.
*/
char * precedence_processing( Token * token, symtable * table, char * key);


/*
*   Funkce odstraní ze zásobníku položku na vrcholu 
*/
int pop(struct Stack* stack);

/*
*   Funkce alokuje paměť pro nový prvek na zásobníku a poté jej vloží 
*/
void push(struct Stack* stack, char * item_name, int item);

/*
*   Funkce zjišťuje, jestli je zásobník prázdný pokud ano, vrací hodnotu 1
*/
int Empty(struct Stack* stack); 

/*
*   Funkce alokuje paměť pro zásobník a inicializuje jej, poté alokuje paměť pro první položku,
    tou bude zarážka a vloží ji na vrchol zásobníku.
*/
struct Stack* firstPush();

/*
*   Funkce uloží do proměnných symbolizující položku ze vstupu a z vrcholu zásobníku index pro vyhlededávání
    v precedenční tabulce.
*/
void get_table_index(Token* token, struct Stack* stack, int* input_index, int* stack_index,int* left_shift);

/*
*   Funkce navrátí index podle daného typu tokenu 
*/
int get_index( int token_type );

/*
*    Funkce získá hodnoty položek z vrcholu zásobníku a ze vstupu, poté pomocí nich vyhledává danou operaci
     v precedenční tabulce. 
*/
char table_value_search(int input_index, int stack_index);

/*
*   Funkce redukuje výraz o dvou operandech na jeden podvýraz a volá funkce, které generují kód.
*/
void expr_eval(struct Stack* stack,int i);

/*
*   Funkce zpracovává podvýrazy, redukuje je a volá funkce, které generují kód symbolizující dané redukce.
*/
void sub_expr_process( struct Stack* stack, symtable * table, char * key ,Token * token);

#define STOP -1 // zarazka $
#define LEFT_OP 30
#define EQ_OP 31
#define TRUE 32
#define FALSE 33
#define E 34
#define BOOL 35
#define T_UNDEFINED 36

#endif