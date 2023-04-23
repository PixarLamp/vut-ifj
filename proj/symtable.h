/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: symtable.c - tabulka symbolov
* Autor: Jakub Vano, xvanoj00
         Zuzana Hrklova, xhrklo00
* ***************************************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include "string.h"

#define SYMTABLE_MAX_SIZE 27487 // Musi byt prvocislo

typedef struct {

    int integer;    // Celociselny datovy typ
    float float_64; // Desetinny datovy typ
    char * string;  // Retezec

} values ;

typedef struct {

    bool global;            // Globalni / lokalni promenna
    values values;          // Pomocne hodnoty
    int t_data;             // Datovy typ nalezici identifikatoru
    bool already_defined;   // Definovano?
    int inputs_no;          // Pocet vstupnich parametru
    int outputs_no;         // Pocet vystupnich (navratovych) parametru
    int in_params[100];     // Datove typy vstupnich parametru
    int out_params[100];    // Datove typy vystupnich parametru

} item_data;

typedef struct list_item {

    char *key;              // Idenifikator promenne
	item_data data;         // Data
	struct list_item *next; // Ukazatel na dalsi symbol

} table_item; 

typedef table_item* symtable[SYMTABLE_MAX_SIZE];

/*
* Hashovaci funkce
* Navraci index
*/
int hash_function(const char *key);

/*
* Inicializace tabulky symbolu
*/
void initialize_symtable(symtable *table);

/*
* Vlozi symbol do tabulky symbolu
* V pripade uspechu navraci hodnotu true
*/
bool insert_symbol(symtable *table, const char *key);

/*
* Vlozi datovy typ promenne do tabulky symbolu
*/
bool insert_data_type (symtable *table, const char *key, int data_type);

/*
* Nastavi priznak definovane promenne v tabulce symbolu
*/
bool set_already_defined(symtable *table, const char *key);

/*
* Nahledne do tabulky symbolu a pokusi se najit symbol pomoci klice
* Navraci data v pripade uspechu, NULL v pripade neuspechu
*/
item_data *search_item(symtable *table, const char *key, const char * var_name);

/*
* Odstrani symbol z tabulky symbolu
* V pripade uspechu navraci true
*/
bool remove_symbol (symtable *table, const char *key);

/*
* Smazani tabulky symbolu
*/
void free_symtable(symtable *table);

#endif