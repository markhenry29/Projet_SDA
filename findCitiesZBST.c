#include "zscore.h"
#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "City.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/* variable global */
double g_latitudeMinzscore;
double g_latitudeMaxzscore;
double g_longitudeMinzscore;
double g_longitudeMaxzscore;


int comparaison_zscore(const void *a, const void *b)
{
    uint64_t* a_ =  (uint64_t*)a;
    uint64_t* b_ =  (uint64_t*)b;

    return (*a_ > *b_) - (*a_ < *b_);
}

bool filtre_zscore(const void* v)
{
    City* c = (City*)v;
    return c->latitude >= g_latitudeMinzscore && c->latitude <= g_latitudeMaxzscore
     && c->longitude >= g_longitudeMinzscore && c->longitude <= g_longitudeMaxzscore;
}

LinkedList* findCitiesZBST(LinkedList* ll,
                           double latitudeMin,
                           double latitudeMax,
                           double longitudeMin,
                           double longitudeMax)
{

    if(ll == NULL)
        return NULL;
    /* construction de l'arbre */
    BinarySearchTree* bst = newBST(comparaison_zscore);
    LLNode* tmp = ll->head;
    g_latitudeMinzscore = latitudeMin;
    g_latitudeMaxzscore = latitudeMax;
    g_longitudeMinzscore = longitudeMin;
    g_longitudeMaxzscore = longitudeMax;
    LinkedList* list_zscore = newLinkedList();

    while(tmp != NULL)
    {
        City* ville = (City*)tmp->value;
        uint64_t *zscore = malloc(sizeof(uint64_t));
        *zscore = zEncode(ville->latitude, ville->longitude);
        insertInBST(bst, zscore, tmp->value);
        insertInLinkedList(list_zscore, zscore);
        tmp = tmp->next;
    }


    /* On va récupérer les villes compris entre 2 zscores */
    uint64_t zscoreMin = zEncode(latitudeMin, longitudeMin);
    uint64_t zscoreMax = zEncode(latitudeMax, longitudeMax);
    /*printf("OK MAINTENANT PLACE AU MIN ET MAX !\n");
    printf("%" PRIu64 "% " PRIu64 "\n", zscoreMin, zscoreMax);*/


    LinkedList* res = getInRange(bst, &zscoreMin, &zscoreMax);
    LinkedList* filtered = filterLinkedList(res, filtre_zscore);

    /* On désalloue les zscores + bst + res */
    freeLinkedList(list_zscore, true);
    freeBST(bst, false);
    freeLinkedList(res, false);

    return filtered;
}
