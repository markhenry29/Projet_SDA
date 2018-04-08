#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "intersect.h"
#include "City.h"
#include "string.h"
#include <stdio.h>
/**
    @return -1 si a < b, 0 si a == b et 1 si a > b
 */
int comparaison_double(const void* a, const void* b)
{
    double *a_ = (double*)a;
    double *b_ = (double*)b;

    return (*a_ > *b_) - (*a_ < *b_);
}

int comparaison_city(const void* a, const void* b)
{
    City* a_ = (City*)a;
    City* b_ = (City*)b;


    return strcmp(a_->name, b_->name);
}


LinkedList* findCities2BST(LinkedList *ll,
                           double latitudeMin,
                           double latitudeMax,
                           double longitudeMin,
                           double longitudeMax)
{

    if(ll == NULL)
        return NULL;
    BinarySearchTree* bstLatitude = newBST(comparaison_double);
    BinarySearchTree* bstLongitude = newBST(comparaison_double);

    LLNode* tmp = ll->head;

    /* On parcours la liste chainé passé en paramètre */
    while(tmp != NULL)
    {

        City* ville = (City*)tmp->value;
        /* on insère dans les 2 arbres selon la latitude et la longitude */
        insertInBST(bstLatitude, &ville->latitude, tmp->value);
        insertInBST(bstLongitude, &ville->longitude, tmp->value);

        tmp = tmp->next;
    }

    /* on crée nos 2 linkedList et on y insère les valeur
    compris entre les longitudes/latitudes passés en param. */
    LinkedList* listeLatitude = getInRange(bstLatitude, &latitudeMin, &latitudeMax);
    LinkedList* listeLongitude = getInRange(bstLongitude, &longitudeMin, &longitudeMax);

    /* on effectue l'intersection des 2 listes */
    LinkedList* intersection = intersect(listeLatitude, listeLongitude, comparaison_city);


    freeLinkedList(listeLatitude, false);
    freeLinkedList(listeLongitude, false);
    freeBST(bstLatitude, false);
    freeBST(bstLongitude, false);

    return intersection;
}
