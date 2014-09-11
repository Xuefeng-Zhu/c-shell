/** @file log.c */
#include <stdlib.h>
#include <string.h>
#include "log.h"

/**
 * Initializes the log.
 *
 * You may assuem that:
 * - This function will only be called once per instance of log_t.
 * - This function will be the first function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @returns
 *   The initialized log structure.
 */
void log_init(log_t *l) {
	l->item = NULL;
	l->prev = NULL;
	l->next = NULL;
}

/**
 * Frees all internal memory associated with the log.
 *
 * You may assume that:
 * - This function will be called once per instance of log_t.
 * - This funciton will be the last function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure to be destoryed.
 */
void log_destroy(log_t* l) {
	if (l->item == NULL)
	{
		return;	
	}

	log_t *p = l->prev;
	while(p != l)
	{
		log_t *temp = p;
		p = p->prev;
		free(temp->item);
		free(temp);
	}
	free(l->item);
}

/**
 * Push an item to the log stack.
 *
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 * @param item
 *    Pointer to a string to be added to the log.
 */
void log_push(log_t* l, const char *item) {
	char *newItem = malloc(strlen(item)+1);
	strcpy(newItem, item);
	if (l->item == NULL)
	{
		l->item = newItem;	
		l->prev = l;
		return;
	}

	log_t *newNode = malloc(sizeof(log_t));
	newNode->item = newItem;
	l->prev->next = newNode;
	newNode->prev = l->prev;
	l->prev = newNode;
	newNode->next = l;
}


/**
 * Preforms a newest-to-oldest search of log entries for an entry matching a
 * given prefix.
 *
 * This search starts with the most recent entry in the log and
 * compares each entry to determine if the query is a prefix of the log entry.
 * Upon reaching a match, a pointer to that element is returned.  If no match
 * is found, a NULL pointer is returned.
 *
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param prefix
 *    The prefix to test each entry in the log for a match.
 *
 * @returns
 *    The newest entry in the log whose string matches the specified prefix.
 *    If no strings has the specified prefix, NULL is returned.
 */
char *log_search(log_t* l, const char *prefix) {
	if (l->item == NULL)
	{
		return NULL;	
	}

	log_t *p = l->prev;
	while (p != l)
	{
		if (strstr(p->item, prefix) == p->item)
		{
			return p->item;	
		}
		p = p->prev;
	}

	if (strstr(p->item, prefix) == p->item)
	{
		return p->item;	
	}
    return NULL;
}
