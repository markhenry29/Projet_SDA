/**
    Fonctions de manipulation des arbres binaires de recherches. (BST)
    @author Richard LE
    @date 08/12/2017
 */
#include "BinarySearchTree.h"
#include "City.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "zscore.h"


/* Il s'agit ici d'un arbre AVL */
struct arbreBinaire_s
{
    void* key;
    void* valeur;
    struct arbreBinaire_s* gauche;
    struct arbreBinaire_s* droite;
    int hauteur;
    int delta;
};
typedef struct arbreBinaire_s ArbreBinaire;


/* structure de BinarySearchTree, il y a 2 champs:
   - un pointeur sur un arbre "*arbre"
   - un pointeur de fonction "int *comparison_fn_t"
 */
struct tree_t
{
    int (*comparison_fn_t)(const void *, const void *);
    ArbreBinaire* arbre;
};

int hauteur(ArbreBinaire* A);
int max(int a, int b);
void freeCity(City* c);
ArbreBinaire* newArbreBinaire(void);
void freeArbreBinaire(ArbreBinaire* arbre, int freeContent);
const void* searchArbreBinaire(ArbreBinaire* arbre, int comparaison(const void*, const void*), const void* key);
size_t sizeOfArbreBinaire(ArbreBinaire* arbre);
void rot_G(ArbreBinaire** A);
void rot_D(ArbreBinaire** A);
bool insertArbreBinaire(ArbreBinaire** A, const void* key, const void* value, int comparaison(const void*, const void*));
void getInRangeArbreBinaire(ArbreBinaire* arbre, void* keyMin, void* keyMax, int comparaison(const void*, const void*), LinkedList* ll);


/*
    Ici les foncitons d'aide pour la résolution des fonctions du projet.
*/
int hauteur(ArbreBinaire* A)
{
    if(A == NULL)
        return -1;

    return A->hauteur;
}

int max(int a, int b)
{
    if(a < b)
        return b;

    return a;
}

void freeCity(City* c)
{
    free(c->name);
    free(c);
}

ArbreBinaire* newArbreBinaire()
{
    ArbreBinaire* res = malloc(sizeof(ArbreBinaire));
    res->key = NULL;
    res->valeur = NULL;
    res->gauche = NULL;
    res->droite = NULL;
    res->hauteur = 0;
    res->delta = 0;

    return res;
}

void freeArbreBinaire(ArbreBinaire* arbre, int freeContent)
{
    if(arbre != NULL)
    {
        freeArbreBinaire(arbre->gauche, freeContent);
        freeArbreBinaire(arbre->droite, freeContent);

        /* si freeContent == 1, on supprime la valeur contenu dans chaque noeud de l'arbre */
        if(freeContent)
            freeCity(arbre->valeur);

        free(arbre);
    }
}

const void* searchArbreBinaire(ArbreBinaire* arbre, int comparaison(const void*, const void*), const void* key)
{

    if(arbre == NULL)
        return NULL;
    /* on va comparer les clés */
    int cmp = comparaison(key, arbre->key);

    if(cmp == 0)
        return arbre->valeur;

    if(cmp < 0)
        return searchArbreBinaire(arbre->gauche, comparaison, key);

    return searchArbreBinaire(arbre->droite, comparaison, key);
}

size_t sizeOfArbreBinaire(ArbreBinaire* arbre)
{
    if(arbre == NULL)
        return 0;

    return 1 + sizeOfArbreBinaire(arbre->gauche) + sizeOfArbreBinaire(arbre->droite);
}

/**
    Rotation gauche si trop d'éléments sont à droite de l'arbre.
 */
void rot_G(ArbreBinaire** A)
{
    if((*A) != NULL && (*A)->droite != NULL)
    {
        ArbreBinaire* tmp = (*A)->droite;
        (*A)->droite = (*A)->droite->gauche;
        tmp->gauche = (*A);
        (*A) = tmp;

        /* maj fils gauche */
        (*A)->gauche->hauteur = 1 + max(hauteur((*A)->gauche->gauche), hauteur((*A)->gauche->droite));

        (*A)->gauche->delta = hauteur((*A)->gauche->gauche) - hauteur((*A)->gauche->droite);

        int h_fg = hauteur((*A)->gauche);
        int h_fd = hauteur((*A)->droite);


        (*A)->hauteur = 1 + max(h_fd, h_fg);
        (*A)->delta = h_fg - h_fd;
    }
}

/**
    Rotation droite.
 */
void rot_D(ArbreBinaire** A)
{
    if((*A) != NULL && (*A)->gauche != NULL)
    {
        /* rotation */
        ArbreBinaire* tmp = (*A)->gauche;
        (*A)->gauche = (*A)->gauche->droite;
        tmp->droite = (*A);
        (*A) = tmp;


        /* maj hauteur + delta fils droit */
        int h_fdfg = hauteur((*A)->droite->gauche);
        int h_fdfd = hauteur((*A)->droite->droite);

        (*A)->droite->hauteur = 1 + max(h_fdfd, h_fdfg);
        (*A)->droite->delta = h_fdfg - h_fdfd;
        /* maj racine*/
        int h_fd = hauteur((*A)->droite);
        int h_fg = hauteur((*A)->gauche);
        (*A)->hauteur = 1 + max(h_fd, h_fg);
        (*A)->delta = h_fg - h_fg;
    }
}

/**
    Insertion d'un élément dans un arbre binaire de recherche.
    Cette fonction a été amoliré pour que ce soit un arbre AVL.
    L'arbre va être réequilibré automatiquement à chaque insertion si nécessaire.
 */
bool insertArbreBinaire(ArbreBinaire** A, const void* key, const void* value, int comparaison(const void*, const void*))
{
    if(*A == NULL)
    {
        ArbreBinaire* arbre = malloc(sizeof(ArbreBinaire));
        arbre->droite = NULL;
        arbre->gauche = NULL;
        arbre->key = (void*)key;
        arbre->valeur = (void*)value;
        arbre->hauteur = 0;
        arbre->delta = 0;
        *A = arbre;
        return true;
    }

    else
    {
        int cmp = comparaison(key, (*A)->key);

        if(cmp <= 0)
            insertArbreBinaire(&(*A)->gauche, key, value, comparaison);

        else
            insertArbreBinaire(&(*A)->droite, key, value, comparaison);


        if(hauteur((*A)->gauche) > hauteur((*A)->droite))
            (*A)->hauteur = 1 + hauteur((*A)->gauche);

        else
            (*A)->hauteur = 1 + hauteur((*A)->droite);


        (*A)->delta = hauteur((*A)->gauche) - hauteur((*A)->droite);


        if((*A)->delta == 2)
        {
            if((*A)->gauche->delta == -1)
                rot_G(&(*A)->gauche);
            rot_D(A);
        }


        else if((*A)->delta == -2)
        {
            if((*A)->droite->delta == 1)
                rot_D(&(*A)->droite);

            rot_G(A);
        }
    }
    return false;
}



void getInRangeArbreBinaire(ArbreBinaire* arbre, void* keyMin, void* keyMax, int comparaison(const void*, const void*), LinkedList* ll)
{
    if(arbre != NULL)
    {
        int cmpMin = comparaison(arbre->key, keyMin);/* cmpMin >= 0 => c'est bon */
        int cmpMax = comparaison(arbre->key, keyMax);/* cmpMax <= 0 => c'est bon */
        int estDansIntervalle = 0;

        if(cmpMin >= 0 && cmpMax <= 0)
            estDansIntervalle = 1;

        /* Si la cle est dans l'intervalle, on l'ajoute dans notre liste (parcours infixe) */
        if(estDansIntervalle == 1)
        {
            getInRangeArbreBinaire(arbre->gauche, keyMin, keyMax, comparaison, ll);
            insertInLinkedList(ll, arbre->valeur);
            getInRangeArbreBinaire(arbre->droite, keyMin, keyMax, comparaison, ll);
        }

        /* Si la cle est en dehors de l'intervalle à valeurs inf, on va à droite */
        if(cmpMin < 0)
            getInRangeArbreBinaire(arbre->droite, keyMin, keyMax, comparaison, ll);

        /* Si cle de l'arbre > max de l'intervalle, on va à gauche */
        if(cmpMax > 0)
            getInRangeArbreBinaire(arbre->gauche, keyMin, keyMax, comparaison, ll);
    }
}
/**
    Les fonctions du projets sont ici.
 */
BinarySearchTree* newBST(int comparison_fn_t(const void *, const void *))
{
    BinarySearchTree* bst = malloc(sizeof(BinarySearchTree));
    bst->arbre = NULL;

    if(bst != NULL)
    {
        bst->comparison_fn_t = comparison_fn_t;
        return bst;
    }

    return NULL;
}

void freeBST(BinarySearchTree* bst, bool freeContent)
{
    if(bst != NULL)
    {
        freeArbreBinaire(bst->arbre, freeContent);
        free(bst);
    }
}

size_t sizeOfBST(const BinarySearchTree* bst)
{
    if(bst == NULL)
        return 0;

    return sizeOfArbreBinaire(bst->arbre);
}

/**
    @return 1 si insertion valide et 0 sinon
 */
bool insertInBST(BinarySearchTree* bst, const void* key, const void* value)
{
    if(bst == NULL)
        return false;


    return insertArbreBinaire(&bst->arbre, key, value, bst->comparison_fn_t);
}

/**

 */
const void* searchBST(BinarySearchTree* bst, const void* key)
{
    if(bst != NULL)
    {
        return searchArbreBinaire(bst->arbre, bst->comparison_fn_t, key);
    }

    return NULL;
}
/*int hhh(ArbreBinaire* A)
{
    if(A == NULL)
        return -1;

    return 1 + max(hhh(A->gauche), hhh(A->droite));
}*/


LinkedList* getInRange(const BinarySearchTree* bst, void* keyMin, void* keyMax)
{
    if(bst == NULL)
        return NULL;
    LinkedList* ll = newLinkedList();
    getInRangeArbreBinaire(bst->arbre, keyMin, keyMax, bst->comparison_fn_t, ll);

    return ll;
}
