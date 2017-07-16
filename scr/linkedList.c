/**
 * \file bitHacks.c
 * \author LUcio Pintanel
 * \date 9 Aug 2016
 * \brief File containing includes, defines, and demias externados.
 *
 */

/***	SESSION INCLUDE	***********************************************/
#include <stdio.h>

#include "../includes/linkedList.h"
/**********************************************************************/

/***	SESSION DEFINE, STRUCT, TYPEDEF	*******************************/
#define MAX_TAM 256UL /*!< Define tamanho maximo de vetor */

#define VERSAO_MAX _VERSAO_MAX  /*!< Definicao de versao Maxima, valor da definicao vem via Makefile */
#define VERSAO_MIN _VERSAO_MIN  /*!< Definicao de versao Minima, valor da definicao vem via Makefile */
#define VERSAO_NRO _VERSAO_NRO  /*!< Definicao de versao Númeor, valor da definicao vem via Makefile */

/**********************************************************************/

/***	SESSION MACROS	***********************************************/

/**********************************************************************/

/***	SESSION VARIAVEIS GLOBAIS	*******************************/

/**********************************************************************/

/***	SESSION PROTOTIPO DE FUNCOES	*******************************/

/**********************************************************************/

/***	SESSION FUNCOES	***********************************************/

/* 
 * \brief Get library version. Function returns version and build number of bitHacks 
 * library. Return value is char pointer. Argument min is flag for output 
 * format. If min is 1, function returns version in full  format, if flag 
 * is 0 function returns only version numbers, For examle: 1.3.0
-*/
const int libEdmVersion(int min, char* vrsStr)
{
	if (NULL == vrsStr) {
		printf("Vetor nao inicializado\n");
		return -1;
	}
	char verstr[MAX_TAM];

	/* Version short */
	if (min) snprintf(verstr, sizeof(verstr), "%d.%d.%d",
		VERSAO_MAX, VERSAO_MIN, VERSAO_NRO);

		/* Version long */
	else snprintf(verstr, sizeof(verstr), "%d.%d build %d (%s)",
		VERSAO_MAX, VERSAO_MIN, VERSAO_NRO, __DATE__);

	return 0;
}

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(stListHead_t *new,
	stListHead_t *prev,
	stListHead_t *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void list_add(stListHead_t *new, stListHead_t *head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(stListHead_t *new, stListHead_t *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(stListHead_t *prev, stListHead_t *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
void list_del(stListHead_t *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
void list_del_init(stListHead_t *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
void list_move(stListHead_t *list, stListHead_t *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
void list_move_tail(stListHead_t *list,
	stListHead_t *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(stListHead_t *head)
{
	return head->next == head;
}

static inline void __list_splice(stListHead_t *list,
	stListHead_t *head)
{
	stListHead_t *first = list->next;
	stListHead_t *last = list->prev;
	stListHead_t *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
void list_splice(stListHead_t *list, stListHead_t *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
void list_splice_init(stListHead_t *list,
	stListHead_t *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

/* end LINKEDLIST_C */