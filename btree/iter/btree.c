/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *tempTreeNode = tree;
    while (tempTreeNode != NULL){
        if(tempTreeNode->key == key){
            *value = tempTreeNode->value;
            return true;
        }
        if(tempTreeNode->key < key){
            tempTreeNode = tempTreeNode->right;
            continue;
        }
        if(tempTreeNode->key > key){
            tempTreeNode = tempTreeNode->left;
            continue;
        }
    }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *tmpTreeNode = *tree, *parentTreeNode = NULL;
    //pokud je strom prazdny, vlozim koren
    if(*tree == NULL){
        *tree = malloc(sizeof (struct bst_node));
        if(*tree == NULL) return;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->key = key;
        (*tree)->value = value;
        return;
    } else{
        //cyklim dokud nenajdu prazdne misto dle key, pak vkladam, pokud klic existuje, update hodnoty
        while (1){
            if(tmpTreeNode == NULL){
                tmpTreeNode = malloc(sizeof (struct bst_node));
                tmpTreeNode->left = NULL;
                tmpTreeNode->right = NULL;
                tmpTreeNode->key = key;
                tmpTreeNode->value = value;
                if(parentTreeNode->key > key) parentTreeNode->left = tmpTreeNode;
                if(parentTreeNode->key < key) parentTreeNode->right = tmpTreeNode;
                return;
            } else {
                if(tmpTreeNode->key == key){
                    tmpTreeNode->value = value;
                    return;
                }
                if(tmpTreeNode->key > key){
                    parentTreeNode = tmpTreeNode;
                    tmpTreeNode = tmpTreeNode->left;
                    continue;
                }
                if(tmpTreeNode->key < key){
                    parentTreeNode = tmpTreeNode;
                    tmpTreeNode = tmpTreeNode->right;
                    continue;
                }
            }
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmpTreeNode = *tree, *parentTreeNode;
    if((*tree)->right == NULL){
        *tree = (*tree)->left;
        free(tmpTreeNode);
        return;
    }
    //prochazim strom dokud nenarazim na uzel nejvice vpravo
    while (tmpTreeNode->right != NULL){
        parentTreeNode = tmpTreeNode;
        tmpTreeNode = tmpTreeNode->right;
    }
    // zmenim hodnoty target
    target->key = tmpTreeNode->key;
    target->value = tmpTreeNode->value;
    //nastavim pointery
    if(tmpTreeNode->left != NULL){
        parentTreeNode->right = tmpTreeNode->left;
    } else {
        parentTreeNode->right = NULL;
    }
    //parentTreeNode->right = tmpTreeNode->left;
    //if(tmpTreeNode->left != NULL) free(tmpTreeNode->left);
    free(tmpTreeNode);
    tmpTreeNode = NULL;
    return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    //ccheck prazndeho stromu
    if(*tree == NULL) return;

    //deklarace
    bst_node_t *tempTreeNode = *tree, *parentTreeNode;
    //promena slouzici pro kontrolu odkud presel strom
    bool leftSide = false;
    //prochazim dokud nenarazim na konec
    while (tempTreeNode != NULL){
        //pokud najdu stejny klic kontroluju potomky, provedu zmenu, return
        if(tempTreeNode->key == key){
            //kdyz nema zadny potomek tak jen uvolnim a nastavim odkaz z parent na null
            if(tempTreeNode->right == NULL && tempTreeNode->left == NULL){
                if(leftSide) parentTreeNode->left = NULL;
                else parentTreeNode->right = NULL;
                free(tempTreeNode);
                tempTreeNode = NULL;
                return;
            }
            //pokud ma jeden potomek, propojim ho s parent
            if(tempTreeNode->right != NULL && tempTreeNode->left == NULL){
                if(leftSide) parentTreeNode->left = tempTreeNode->right;
                else parentTreeNode->right = tempTreeNode->right;
                free(tempTreeNode);
                tempTreeNode = NULL;
                return;
            }
            if(tempTreeNode->right == NULL && tempTreeNode->left != NULL){
                if(leftSide) parentTreeNode->left = tempTreeNode->left;
                else parentTreeNode->right = tempTreeNode->left;
                free(tempTreeNode);
                tempTreeNode = NULL;
                return;
            }
            //pro odstraneni s dvema potomky zavolam pomocnou fci
            if(tempTreeNode->right != NULL && tempTreeNode->left != NULL){
                bst_replace_by_rightmost(tempTreeNode, &tempTreeNode->left);
                return;
            }
        }
        if(tempTreeNode->key < key){
            leftSide = false;
            parentTreeNode = tempTreeNode;
            tempTreeNode = tempTreeNode->right;
            continue;
        }
        if(tempTreeNode->key > key){
            leftSide = true;
            parentTreeNode = tempTreeNode;
            tempTreeNode = tempTreeNode->left;
            continue;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    //strom je prazdny
    if(*tree == NULL) return;

    //stack init
    stack_bst_t stack;
    stack_bst_init(&stack);

    // push pocatku stromu
    stack_bst_push(&stack,*tree);

    //dokud neni stack prazdny, tak cyklim
    while (!stack_bst_empty(&stack)){
        //vezmu z vrcholu, popnu, pokud ma node potomky, pridam do stacku a uvolnim node
        bst_node_t *delTreeNode = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        if(delTreeNode->right != NULL) stack_bst_push(&stack, delTreeNode->right);
        if(delTreeNode->left != NULL) stack_bst_push(&stack, delTreeNode->left);
        free(delTreeNode);
    }
    //Re-Init
    *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *tempTreeNode = tree;
    while (tempTreeNode != NULL){
        bst_print_node(tempTreeNode);
        stack_bst_push(to_visit, tempTreeNode);
        tempTreeNode = tempTreeNode->left;
    }

}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t preStack;
    bst_node_t *tempTreeNode;
    //if(tree == NULL) return;
    stack_bst_init(&preStack);
    bst_leftmost_preorder(tree,&preStack);
    while (!stack_bst_empty(&preStack)){
        tempTreeNode = stack_bst_pop(&preStack);
        bst_leftmost_preorder(tempTreeNode->right, &preStack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *tempTreeNode = tree;
    while (tempTreeNode != NULL){
        stack_bst_push(to_visit,tempTreeNode);
        tempTreeNode = tempTreeNode->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t inStack;
    bst_node_t *tempTreeNode;
    if(tree == NULL) return;
    stack_bst_init(&inStack);
    bst_leftmost_inorder(tree,&inStack);
    while (!stack_bst_empty(&inStack)){
        tempTreeNode = stack_bst_top(&inStack);
        stack_bst_pop(&inStack);
        bst_print_node(tempTreeNode);
        bst_leftmost_inorder(tempTreeNode->right, &inStack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    bst_node_t *tempTreeNode = tree;
    while(tempTreeNode != NULL){
        stack_bst_push(to_visit,tempTreeNode);
        stack_bool_push(first_visit,tempTreeNode);;
        tempTreeNode = tempTreeNode->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */

void bst_postorder(bst_node_t *tree) {
    stack_bst_t postStack;
    stack_bool_t postBoolStack;
    //bst_node_t *tempTreeNode = tree ;
    bool fVisit;

    stack_bst_init(&postStack);
    stack_bool_init(&postBoolStack);
    bst_leftmost_postorder(tree, &postStack, &postBoolStack);

    while (!stack_bst_empty(&postStack)){
        bst_node_t *tempTreeNode = stack_bst_pop(&postStack);
        //stack_bst_pop(&postStack);
        fVisit = stack_bool_pop(&postBoolStack);
        //stack_bool_pop(&postBoolStack);

        if(fVisit){
            stack_bst_push(&postStack, tempTreeNode);
            stack_bool_push(&postBoolStack, false);
            bst_leftmost_postorder(tempTreeNode->right, &postStack, &postBoolStack);
            continue;
        }
        bst_print_node(tempTreeNode);
    }

}
