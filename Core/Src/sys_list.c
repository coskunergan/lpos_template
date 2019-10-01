/*
 *  Link-List Utility Library
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "sys_list.h"

struct list
{
    struct list *next;
};

/*********************************************************/
/*********************************************************/
/*********************************************************/
void SysList_Init(list_t list)
{
    *list = NULL;
}
/*********************************************************/
void *SysList_Head(list_t list)
{
    return *list;
}
/*********************************************************/
void SysList_Copy(list_t dest, list_t src)
{
    *dest = *src;
}
/*********************************************************/
void *SysList_Tail(list_t list) // listenin en son elemanini çagirir. Null ise eleman yok demektir.
{
    struct list *l;

    if(*list == NULL)
    {
        return NULL;
    }

    for(l = *list; l->next != NULL; l = l->next);

    return l;
}
/*********************************************************/
void SysList_Add(list_t list, void *item)// öncelikle elemani listeden siler ve liste sonuna eleman ekler.
{
    struct list *l;

    SysList_Remove(list, item);

    ((struct list *)item)->next = NULL;

    l = SysList_Tail(list);

    if(l == NULL)
    {
        *list = item;
    }
    else
    {
        l->next = item;
    }
}
/*********************************************************/
void SysList_Push(list_t list, void *item) // öncelikle girilen eleman adresini listeden çikarir. adresi girilen elemani listenin basina ekler.
{
    SysList_Remove(list, item);

    ((struct list *)item)->next = *list;
    *list = item;
}
/*********************************************************/
void *SysList_Chop(list_t list) // listenin son elemanini koparip ptr adresi ile beraber döner.(egerki listede 1 eleman varsa onu koparip adresi ile döner)
{
    struct list *l, *r;

    if(*list == NULL)
    {
        return NULL;
    }
    if(((struct list *)*list)->next == NULL)
    {
        l = *list;
        *list = NULL;
        return l;
    }

    for(l = *list; l->next->next != NULL; l = l->next);

    r = l->next;
    l->next = NULL;

    return r;
}
/*********************************************************/
void *SysList_Pop(list_t list)// listenin ilk elemanini koparip ptr adresi ile döner.
{
    struct list *l;
    l = *list;
    if(*list != NULL)
    {
        *list = ((struct list *)*list)->next;
    }

    return l;
}
/*********************************************************/
void SysList_Remove(list_t list, void *item) // listede adresi girilen elemani koparir.
{
    struct list *l, *r;

    if(*list == NULL)
    {
        return;
    }
    r = NULL;
    for(l = *list; l != NULL; l = l->next)
    {
        if(l == item)
        {
            if(r == NULL)
            {
                *list = l->next;
            }
            else
            {
                r->next = l->next;
            }
            l->next = NULL;
            return;
        }
        r = l;
    }
}
/*********************************************************/
int SysList_Length(list_t list) // listenin eleman sayisini verir.
{
    struct list *l;
    int n = 0;

    for(l = *list; l != NULL; l = l->next)
    {
        ++n;
    }
    return n;
}
/*********************************************************/
void SysList_Insert(list_t list, void *previtem, void *newitem) // listede girilen bi önceki eleman adresinin sonrasina eleman ekler.
{
    if(previtem == NULL)
    {
        SysList_Push(list, newitem);
    }
    else
    {
        ((struct list *)newitem)->next = ((struct list *)previtem)->next;
        ((struct list *)previtem)->next = newitem;
    }
}
/*********************************************************/
void *SysList_Item_Next(void *item)// listenin bir sonraki elemanina geçer.
{
    return item == NULL ? NULL : ((struct list *)item)->next;
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

