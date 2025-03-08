#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *dummy = malloc(sizeof(struct list_head));
    if (!dummy)
        return NULL;
    INIT_LIST_HEAD(dummy);
    return dummy;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *trav = NULL, *safe = NULL;

    list_for_each_entry_safe (trav, safe, head, list) {
        list_del(&trav->list);
        q_release_element(trav);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    INIT_LIST_HEAD(&new->list);
    new->value = strdup(s);
    if (!new->value)
        return false;

    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    INIT_LIST_HEAD(&new->list);
    new->value = strdup(s);
    if (!new->value)
        return false;

    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    else {
        struct list_head *enemy = head->next;
        element_t *removed_e = list_entry(enemy, element_t, list);
        list_del(&removed_e->list);
        strncpy(sp, removed_e->value, bufsize - 1);
        return removed_e;
    }
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    else {
        struct list_head *enemy = head->prev;
        element_t *removed_e = list_entry(enemy, element_t, list);
        list_del(&removed_e->list);
        strncpy(sp, removed_e->value, bufsize - 1);
        return removed_e;
    }
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int count = 0;
    struct list_head *trav;

    list_for_each (trav, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    void swap2node(struct list_head * node1, struct list_head * node2)
    {
        struct list_head *temp1 = node1->prev;
        struct list_head *temp2 = node2->next;
        node1->next = temp2;
        node1->prev = node2;
        node2->next = node1;
        node2->prev = temp1;
        temp1->next = node2;
        temp2->prev = node1;
        return;
    }
    struct list_head *trav = NULL;
    struct list_head *safe = NULL;
    int count = 0;
    list_for_each_safe (trav, safe, head) {
        count++;
        if (count == 2) {
            swap2node(trav->prev, trav);
            count = 0;
        }
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *trav = NULL;
    struct list_head *safe = NULL;
    list_for_each_safe (trav, safe, head) {
        trav->next = trav->prev;
        trav->prev = safe;
    }
    struct list_head *temp = NULL;
    temp = head->next;
    head->next = head->prev;
    head->prev = temp;
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    // Guard Clause
    if (!head || list_empty(head) || list_is_singular(head) || k < 2)
        return;
    //
    LIST_HEAD(reversek);
    LIST_HEAD(buffer);
    int count = 0;
    //
    struct list_head *trav, *safe;
    list_for_each_safe (trav, safe, head) {
        count++;
        if (count == k) {
            list_cut_position(&reversek, head, trav);
            count = 0;
            q_reverse(&reversek);
            list_splice_init(&reversek, (&buffer)->prev);
        }
    }
    list_splice_init(&buffer, head);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
