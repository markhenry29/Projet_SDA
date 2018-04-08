#ifndef RECHERCHE_H
#define RECHERCHE_H


int comparaison_double(const void* a, const void* b);
int comparaison_city(const void* a, const void* b);
bool filtreLongitude(const void* v);


LinkedList* findCities2BST(LinkedList *ll,
                           double latitudeMin,
                           double latitudeMax,
                           double longitudeMin,
                           double longitudeMax);

LinkedList* findCities1BST(LinkedList *ll,
                           double latitudeMin,
                           double latitudeMax,
                           double longitudeMin,
                           double longitudeMax
                           );
LinkedList* findCitiesZBST(LinkedList* ll,
                           double LongitudeMin,
                           double longitudeMax,
                           double latitudeMin,
                           double latitudeMax);
#endif // RECHERCHE_H
