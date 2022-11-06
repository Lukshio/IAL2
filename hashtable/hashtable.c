/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; ++i) {
       (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    ht_item_t *tmpItem = (*table)[get_hash(key)];
    while (tmpItem != NULL){
        if(tmpItem->key == key) return tmpItem;
        tmpItem = tmpItem->next;
    }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    //kontrola jestli tam prvek jiz neni
    ht_item_t *tmpItem = ht_search(table, key);
    if(tmpItem != NULL){
        tmpItem->value = value;
    } else {
        tmpItem = malloc(sizeof(ht_item_t*));
        if(tmpItem == NULL) return;
        tmpItem->value = value;
        tmpItem->key = key;
        tmpItem->next = (*table)[get_hash(key)];
        (*table)[get_hash(key)] = tmpItem;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    return &(ht_search(table, key))->value;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    ht_item_t *delItem, *prevItem;
    if(table == NULL || key == NULL) return;
    prevItem = (*table)[get_hash(key)];
    if(prevItem != NULL){
        if(prevItem->key == key){
            free(prevItem);
            prevItem = NULL;
            return;
        } else {
            while (prevItem->next != NULL){
                if(prevItem->next->key == key){
                    delItem = prevItem->next;
                    prevItem->next = delItem->next;
                    free(delItem);
                    delItem = NULL;
                } else {
                    prevItem = prevItem->next;
                }
            }
            return;
        }
    } else {
        return;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    ht_item_t *delItem, *tmpItem;
    if(table == NULL) return;
    for (int i = 0; i < HT_SIZE; i++) {
        tmpItem = (*table)[i];
        while (tmpItem != NULL){
            delItem = tmpItem;
            tmpItem = tmpItem->next;
            free(delItem);
            delItem = NULL;
        }
        (*table)[i] = NULL;
    }
}
