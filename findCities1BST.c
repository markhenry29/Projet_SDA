#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "intersect.h"
#include "City.h"
#include "string.h"
#include "recherche.h"

/* variable global */
double g_longitudeMax;
double g_longitudeMin;


bool filtreLongitude(const void* v)
{
    City* c = (City*)v;
    return c->longitude >= g_longitudeMin && c->longitude <= g_longitudeMax;
}

LinkedList* findCities1BST(LinkedList *ll,
                           double latitudeMin,
                           double latitudeMax,
                           double longitudeMin,
                           double longitudeMax
                           )
{

    if(ll == NULL)
        return NULL;
    BinarySearchTree* bstLatitude = newBST(comparaison_double);
    g_longitudeMax = longitudeMax;
    g_longitudeMin = longitudeMin;

    LLNode* tmp = ll->head;
    while(tmp != NULL)
    {
        City* ville = (City*)tmp->value;
        insertInBST(bstLatitude, &ville->latitude, tmp->value);
        tmp = tmp->next;
    }


    LinkedList* listeLatitude = getInRange(bstLatitude, &latitudeMin, &latitudeMax);

    LinkedList* filtered = filterLinkedList(listeLatitude, filtreLongitude);

    freeBST(bstLatitude, false);
    return filtered;
}
