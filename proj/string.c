/*****************************************************************
* Projekt: Implementace prekladace imperativniho jazyka IFJ20
* Varianta: II
* Tym: 094
*
* Soubor: string.c - dynamicky retezec
* Autor: Ondrej Sebesta, xsebes22
* ***************************************************************/

#include "string.h"

void initialize_string( char ** string )
{

    // Alokace pameti
    char * tmp_string = malloc( 2 * sizeof( char ) );

    // Osetreni alokace pameti
    if ( tmp_string == NULL ) {
        fprintf( stderr, "Chyba > Alokace pameti pro dynamicky string selhala!" );
        exit( INTERN_ERR );
    }

    // Nastaveni adresy retezce na nove alokovanou pamet
    *string = tmp_string;

}

void append_string( char ** string, char character )
{    
    // Realokace pameti pro novou delku retezce (+1 znak)
    long unsigned int size = sizeof(&string) * 10;
    *string = realloc( *string, size );
    
    // Osetreni realokace pameti
    if ( *string == NULL ) {
        fprintf( stderr, "Chyba > Realokace pameti pro dynamicky string selhala!" );
        exit( INTERN_ERR );
    }
    
    // Vlozeni znaku na konec retezce
    strncat( *string, &character, 1 );

}

void renew_string( char ** string, char * new_string ) {

    // Realokace pameti pro novy retezec a prekopirovani retezce
    *string = realloc( *string, sizeof(new_string));
    memcpy(*string,new_string,sizeof(&new_string));

}

void free_string( char ** string )
{
    
    // Uvolneni pameti retezce
    free( *string );

}