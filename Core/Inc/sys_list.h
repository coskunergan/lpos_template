/*
 *  Link-List Utility Library
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef SYS_LIST_H_
#define SYS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define LIST_CONCAT2(s1, s2) s1##s2
#define LIST_CONCAT(s1, s2) LIST_CONCAT2(s1, s2)


#define LIST(name) \
         static void *LIST_CONCAT(name,_list) = NULL; \
         static list_t name = (list_t)&LIST_CONCAT(name,_list)


#define LIST_STRUCT(name) \
         void *LIST_CONCAT(name,_list); \
         list_t name


#define LIST_STRUCT_INIT(struct_ptr, name)                              \
    do {                                                                \
       (struct_ptr)->name = &((struct_ptr)->LIST_CONCAT(name,_list));   \
       (struct_ptr)->LIST_CONCAT(name,_list) = NULL;                    \
       SysList_Init((struct_ptr)->name);                                   \
    } while(0)

typedef void **list_t;

void  SysList_Init(list_t list);
void *SysList_Head(list_t list);
void *SysList_Tail(list_t list);
void *SysList_Pop(list_t list);
void  SysList_Push(list_t list, void *item);
void *SysList_Chop(list_t list);
void  SysList_Add(list_t list, void *item);
void  SysList_Remove(list_t list, void *item);
int   SysList_Length(list_t list);
void  SysList_Copy(list_t dest, list_t src);
void  SysList_Insert(list_t list, void *previtem, void *newitem);
void *SysList_Item_Next(void *item);

#ifdef __cplusplus
}
#endif		
		
#endif /* SYS_LIST_H_ */
