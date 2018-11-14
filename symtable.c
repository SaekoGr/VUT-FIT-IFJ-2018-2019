
/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "error.h"


#define HTSIZE 6421
#define STACK_SIZE 30
#define TRUE 1
#define FALSE 0

/*datové typy*/

typedef enum
{
  NILL,
  STRING,
  INTEGER,
  PRASATKO_S_PAPUCKAMI_FLT,
  FUNCTION,

} Type_of_tHTItem;


 typedef struct tHTItem{
  char* key;       
  Type_of_tHTItem typ;    
  bool defined;
  int param_count;
  //Token_attr data;
  struct tHTItem* ptrnext;
} tHTItem;

typedef tHTItem* tHTable[HTSIZE];

/* Hlavičky funkcií. */

int sym_table_error(int error_code);
int hashCode ( char key[] );
void htInit ( tHTable ptrht );
void STinits();
tHTItem* htSearch ( tHTable ptrht, char key[] );
int def_ID( tHTable ptrht,char key[] );
int htInsert ( tHTable ptrht, tHTItem* item_ptr );
Type_of_tHTItem* get_type (tHTable ptrht,char key[]);
int check_define (char key[]);
void htClearAll ( tHTable ptrht );
int STlast_check();

