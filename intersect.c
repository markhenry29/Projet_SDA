#include "intersect.h"
#include "LinkedList.h"


LinkedList* intersect(const LinkedList* listA, const LinkedList* listB,
                      int comparison_fn_t(const void *, const void *))
{
    LinkedList* intersection = newLinkedList();

    /* Si A ou B est NULL */
    if(!listA || !listB)
    {
        freeLinkedList(intersection, false);
        return NULL;
    }



    /* si les 2 listes ne sont pas NULL */
    else
    {
        LLNode* currentA = listA->head;
        while(currentA != NULL)
        {
            LLNode* currentB = listB->head;
            while(currentB != NULL)
            {
                /* On compare ici les valeurs contenues dans A et B */
                if(comparison_fn_t(currentA->value, currentB->value) == 0)
                {
                    /* on insère dans la nouvelle liste qui est l'intersection de A B */
                    insertInLinkedList(intersection, currentA->value);
                }

                currentB = currentB->next;
            }
            currentA = currentA->next;
        }

        return intersection;
    }
}
