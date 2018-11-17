/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

// VARIANTA HASH TABLE

#include "symtable.h"


const char* types_of_HTitem[] = {
	"NILL",
	"STRING",
	"INTEGER",
	"PRASATKO_S_PAPUCKAMI_FLT",
	"VAR",
	"FUNCTION",
};

const char* is_defined[] = {
	"FALSE",
	"TRUE",
};


/* ladiaca funkcia pre ST*/


void htPrintTable( tHTable ptrht ) {
	if (ptrht == global_ST)
		printf ("------------HASH TABLE - GLOBAL-------------\n");
	else
		printf ("------------HASH TABLE - LOCAL-------------\n");

	for ( int i=0; i<HTSIZE; i++ ) {
		if ((ptrht)[i]==NULL)
		{
			continue;
		}
		printf ("%i:",i);
		tHTItem* ptr = (ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s,%s,%s,%d)\n",ptr->key, types_of_HTitem[ptr->typ], is_defined[ptr->defined],ptr->param_count);
			ptr = ptr->ptrnext;
		}
		//printf("\n");
	}
	printf ("------------koniec tabulky------------\n");
}

/**
 * Writes error to stderr, returns error code
 */

int sym_table_error(int error_code){
	htClearAll(local_ST);
	//htInit(local_ST);
	htClearAll(global_ST);
	//htInit(global_ST);
	fprintf(stderr, "sym_table ERROR\n");
return error_code;
}


/*Funkcia vracia index tabulky na zaklade kluca
**Funkcia je prebraná z 2. domácej úlohy IAL
*/
int hashCode ( char key[] ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}


/*Funkcia inicializuje kazdú položku tabuľky na NULL
**Využíva sa len vo funkciách htInits() a htClearAll()
*/

void htInit ( tHTable ptrht ) {

	for (int i=0;i<HTSIZE;i++){
			(ptrht)[i]=NULL;
		}
}

/*
** Funkcia inicializuje obe tabulky
*/
void STinits(){
	htInit(global_ST);
	htInit(local_ST);
}

/* Hashovacia tabuľka s explicitne zreťazenými synonymami.
** Vyhľadávanie prvku v hashovacej tabuľke ptrht podľa zadaného kľúča key. 
** Ak je daný prvok nájdený, vracia pointer na daný prvok. 
** Ak sa položka v tabuľke nenachádza funkcia vracia NULL.
**
*/
tHTItem* htSearch ( tHTable ptrht, char key[] ) {

	tHTItem *tmp=(ptrht)[hashCode(key)];

	while (tmp){							
	
		if (strcmp(tmp->key, key)==0){		
			return tmp;
		}
		if (tmp->ptrnext==NULL){			
			return NULL;
		}
		else{
			tmp=tmp->ptrnext;				
		}
	}
return NULL;
}

/* Funkcia vkladá do zvolenej tabuľky ptrht položku key, ktorá má typ NILL
** Pri chybe alokácie vracia hodnotu ER_INTERNAL
** Pri pokuse o redefiniciu premennej vracia ER_SEM_VARIABLE
** Inak vracia 0
*/

int def_ID( tHTable ptrht,char key[] ){

	if (htSearch(ptrht,key)!=NULL){							//ak je položka nájdená jedná sa o ER_SEM_VARIABLE
		
		return sym_table_error(ER_SEM_VARIABLE);
 		 
 	}
 	else{
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));			//alokácia novej položky
 		if (Ninsert==NULL){								
 			return sym_table_error(ER_INTERNAL);

 		}
 		
		Ninsert->key = (char*) malloc((strlen(key)+2));		//alokácia kľúča
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,key);							//inicializácia položky
 		Ninsert->typ=VAR;	
 		Ninsert->param_count=0;
 		Ninsert->defined=true;						
 		Ninsert->ptrnext=(ptrht)[hashCode(key)];			//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(key)]=Ninsert;
 	}
	return ST_OK;
}

/* Funkcia vkladá do zvolenej tabuľky ptrht celú vybranú položku item_ptr 
** Ak je položka už v tabuľke aktualizuje jej typ 
** v pripade FUNCTION môže preklopiť jej defined zložku
*/

int htInsert ( tHTable ptrht, tHTItem* item_ptr ) {


	tHTItem *actual_item=htSearch(ptrht,item_ptr->key);
	if (actual_item){													//ak je item najdeny aktuelizuje data
		if ((item_ptr->typ==FUNCTION)&&(actual_item->typ==FUNCTION)){	//obe su funkcie
			if (actual_item->param_count==item_ptr->param_count){		//maju rovnaky pocet parametrov
				if (item_ptr->defined && !actual_item->defined){		//item v tabzľke je nedefinovany a "pridávaný" item je definovany
					actual_item->defined=item_ptr->defined;
					return ST_OK;
				}
			}
		}
		if (item_ptr->defined == false){
			return sym_table_error(ER_SEM_VARIABLE);
		}else{
			actual_item->defined= true;
		}
/*
 		switch(item_ptr->typ){
 			case STRING:
					actual_item->typ= item_ptr->typ;
					actual_item->defined= item_ptr->defined;
					//clear_string_content(actual_item->data.string);
	 				//copy_string_content(actual_item->data.string, item_ptr->data.string);
				 	break;
				case INTEGER:
					actual_item->defined= item_ptr->defined;
					actual_item->typ= item_ptr->typ;
					//actual_item->data.integer=item_ptr->data.integer;
					break;
				case PRASATKO_S_PAPUCKAMI_FLT:
					actual_item->defined= item_ptr->defined;
					actual_item->typ= item_ptr->typ;
					//actual_item->data.PRASATKO_S_PAPUCKAMI_FLT=item_ptr->data.PRASATKO_S_PAPUCKAMI_FLT;
					break;
				default:
					return  sym_table_error(ER_SEM_VARIABLE);
 					break;
 		}*/
 	}
 	else{																//položka nie je v tabuľke

 		tHTItem *Ninsert = malloc(sizeof(tHTItem));						//alokácia položky
 		if (Ninsert==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

		Ninsert->key = (char*) malloc((strlen((item_ptr->key))+2));		//alokácia kľúča
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,item_ptr->key);								//nahrajú sa kľúč a data
 		Ninsert->typ=item_ptr->typ;							
 		Ninsert->param_count=item_ptr->param_count;
 		Ninsert->defined=item_ptr->defined;		
 		Ninsert->ptrnext=(ptrht)[hashCode(item_ptr->key)];				//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(item_ptr->key)]=Ninsert;
 	}
 return ST_OK;
}

/* Funkcia vyhľadáva v zvolenej tabuľke ptrht položku s kľúčom key
** Ak je položka nájdená vracia ukazateľ na jej typ
** Ak sa položka v tabuľke nenacjhádza vracia ukazateľ na NULL
*/

Type_of_tHTItem* get_type (tHTable ptrht,char key[]) {

	printf("som v get_type\n");
	printf("%s\n",key);
	if (htSearch(ptrht,key)!=NULL){				//ak je item najdeny
		return &(htSearch(ptrht,key))->typ;		//funkcia vrati typ hladaneho itemu
	}
	else{										//inak vrati NULL
		return NULL;
	}
}

/* Funkcia kontroluje či je funkcia s kľúčom key v globálnej tabuľke symbolov (global_ST) definovaná
** Návratové hodnoty
** NOT_FOUND==2
** param_defined==3
** param_undefined==4
** defined==FALSE==0
** defined==TRUE==1
*/

int check_define (tHTable tab, char key[]) {

	tHTItem *tmp = htSearch(tab, key);
	if (tmp==NULL){
		return NOT_FOUND;
	}

	if ((tmp->typ)!=FUNCTION){
		if (tmp->defined){
			return PARAM_DEFINED;
		}else{
			return PARAM_UNDEFINED;
		}			
	}else{
		if (tmp->defined) 
		{
			return FUNCTION_DEFINED;
		}else 
			return FUNCTION_UNDEFINED;
	}								
}

/* Vyčistí tabuľku ptrht a reinicializuje ju
*/

void htClearAll ( tHTable ptrht ) {



	tHTItem *tmp;
	for (int i=0;i<HTSIZE;i++){
		
		tmp=(ptrht)[i];

		while((ptrht)[i]!=NULL){
			tmp=(ptrht)[i];
			(ptrht)[i]=(ptrht)[i]->ptrnext;
			free(tmp->key);
			free(tmp);
		}
	}
	htInit(ptrht);							//reinicializacia tabulky
}

/* Skontroluje či sú všetky funkcie v Globálnej tabuľke symbolov definovane
** Ak narazí na nedefinovanú funkciu skončí s návratovou hodnotou ER_SEM_VARIABLE
** Ak sú všetky funkcie definované vracia hodnotu 0
*/

int STlast_check(){

	tHTItem *tmp;
	
	for (int i=0;i<HTSIZE;i++){	
		
		tmp=global_ST[i];

		while(tmp!=NULL){
			printf("%s\n",tmp->key );
			if ((tmp->typ==FUNCTION)&&(tmp->defined==false)){
				return sym_table_error(ER_SEM_VARIABLE);
			}
			tmp=tmp->ptrnext;
		}
	}
	return ST_OK;
}

void htClearAlltables(){
	htClearAll(local_ST);
	htClearAll(global_ST);

}

void htClearlocal () {



	tHTItem *tmp;
	for (int i=0;i<HTSIZE;i++){
		
		tmp=(local_ST)[i];

		while((local_ST)[i]!=NULL){
			tmp=(local_ST)[i];
			(local_ST)[i]=(local_ST)[i]->ptrnext;
			free(tmp->key);
			free(tmp);
		}
	}
	htInit(local_ST);							//reinicializacia tabulky
}

int iteminit(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc ){

	if (item==NULL){							
		return sym_table_error(ER_INTERNAL);
	}
	item->key = (char*) malloc((strlen(k)+2));
	if (item->key==NULL){								//ak sa alokacia nepodarila tak funkcia skonci
			return sym_table_error(ER_INTERNAL);
	}

	strcpy(item->key,k);
	item->typ=t;	
	item->param_count=pc;
	item->defined=d;						
	item->ptrnext=NULL;
return ST_OK;

}

int itemupdate(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc ){
	printf("Checkpoint1\n");
	free(item->key);
	printf("Checkpoint2\n");
	item->key = (char*) malloc((strlen(k)+2));
	printf("Checkpoint3\n");
	if (item->key==NULL){								//ak sa alokacia nepodarila tak funkcia skonci
			printf("Checkpoint4\n");
			return sym_table_error(ER_INTERNAL);
	}

	printf("Checkpoint5\n");
	strcpy(item->key,k);
	item->typ=t;	
	item->param_count=pc;
	item->defined=d;						
	item->ptrnext=NULL;
return ST_OK;

}

void itemfree(tHTItem* item){
	free(item->key);
}

/*
*/

int check_param_cnt(char key[]){


	//NOT_FOUND kontrolovany parserom
	//tHTItem *tmp = htSearch(global_ST, key);
	return htSearch(global_ST, key)->param_count;

}
