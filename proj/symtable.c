/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: symtable.c -tabulka symbolov
* Autor: Jakub Vano, xvanoj00
         Zuzana Hrklova, xhrklo00       
* ***************************************************************/

// [1] https://www.partow.net/programming/hashfunctions/  

#include <stdlib.h>
#include <string.h>
#include "symtable.h"

// Funkce inspirovana implementaci DJB hash z [1]
int hash_function(const char *key)
{
    unsigned long hash = 5381;
    int ch;

    while ((ch = *key++)) {
      hash = ((hash << 5) + hash) + ch;
    }
        
    return (hash % SYMTABLE_MAX_SIZE);

}

void initialize_symtable(symtable *table)
{

  if (table == NULL)
		return;

  for(int i = 0; i < SYMTABLE_MAX_SIZE; i++){
		(*table)[i] = NULL; //nastavenie vsetkych hodnot v tabulke na NULL
	}

}

bool insert_symbol(symtable *table, const char *key)
{

  if (table == NULL || key == NULL) {
    return false;
  }

  int index = hash_function(key); //zistenie indexu

  table_item *new_item = (table_item*)malloc(sizeof(table_item));
  
  if (!new_item){
    return false; //alokacia zlyhala
  }

  item_data *searched_item = search_item(table, key,key);

  if (searched_item != NULL){
    free(new_item); //uvolnenie alokovanej pamate
    return false; //symbol sa uz v tabuke nachadza
  }

  initialize_string(&new_item->key);
  initialize_string(&new_item->data.values.string);

  //vkladany symbol nema ziadne synonyma
  if ((*table)[index] == NULL) {

    (*table)[index] = new_item;
    new_item->next = NULL;

  } else {
  
    new_item->next = (*table)[index]; //symbol ma v tabulke synonyma, nastavenie ukazovatela

  }
  
  int length = (int)strlen(key);
  for (int i = 0; i < length; i++){
    append_string( &new_item->key, key[i]); //zapis kluca do struktury
  }

  new_item->data.global = false;
  new_item->data.already_defined = false;
  new_item->data.t_data = -1;
  return true;
}

bool insert_data_type (symtable *table, const char *key, int data_type)
{
  if(table == NULL || key == NULL){
    return false;
  }

  int index = hash_function(key);
  item_data *item_modified = search_item(table, key,key);
  if ( item_modified == NULL ) {
      return false; //symbol sa v tabulke nenachadza
  }
  item_modified->t_data = data_type;
  return true;
}

bool set_already_defined(symtable *table, const char *key)
{
  if(table == NULL || key == NULL){
    return false;
  }

  int index = hash_function(key);
  item_data *item_modified = search_item(table, key, key);
  item_modified->already_defined = true;
  return true;
}

item_data *search_item(symtable *table, const char *key,const char * var_name)
{
  if (table == NULL || key == NULL){
    return NULL;
  }

  int index = hash_function(key);

  table_item *item = (*table)[index];

  //prechadza cely linearny zoznam na danom indexe
  for (; item != NULL ; item = item->next) {
    
    if ( !strcmp(item->key, var_name ) ){
      //symbol sa nachadza v tabulke
      return &item->data;

    }

  }

  //symbol sa nenasiel v tabulke
  return NULL;

}

bool remove_symbol (symtable *table, const char *key)
{
  if (table == NULL || key == NULL){
    return false;
  }
  item_data *searched_item = search_item(table, key, key);
  if (searched_item == NULL){
    return false; //symbol sa nenachadza v tabulke, nie je co mazat
  }

  int index = hash_function(key);
  table_item *item = (*table)[index];
  table_item * tmp_ptr = NULL;

  //prechaza zoznamom na danom indexe
  for (; item; item = item->next ){
    //najde ziadany symbol
    if( !strcmp(item->key, key ) ){
      
      //prenastavenie ukazovatela v linearnom zozname
      tmp_ptr = item->next;
      if ( tmp_ptr == NULL ) {

        (*table)[index] = (*table)[index]->next;

      } else {

        tmp_ptr->next = item->next;

      }

      //uvolnenie alokovanej pamate
      free_string(&item->data.values.string);
      free_string(&item->key);
      free(item);

      return true;

    }
    tmp_ptr = item;
  }

  return false;

}

void free_symtable(symtable *table)
{
  if (table == NULL){
    return;
  }

  //prechadza kazdy index v tabulke
  for(int i = 0; i < SYMTABLE_MAX_SIZE; i++){

		table_item * item = (*table)[i];
    table_item * tmp_ptr = NULL;

    //prechadza kazdy prvok linearneho zoznamu
		for ( ; item != NULL; item = tmp_ptr ) {

        tmp_ptr = item->next;

        if ( item != NULL ) {        
          free( item ); //uvolnenie alokovanej pamate
        }
        

		}

		(*table)[i] = NULL; //nastavenie tabulky do pociatocneho stavu

	}

  return;

}



