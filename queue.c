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
    element_t *trav, *safe;
    list_for_each_entry_safe(trav, safe, head, list) {
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
    if (!new)
        return false;
    INIT_LIST_HEAD(&new->list);
    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    INIT_LIST_HEAD(&new->list);
    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }
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
        if (sp) {
            strncpy(sp, removed_e->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        }
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
        if (sp) {
            strncpy(sp, removed_e->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        }
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

    list_for_each(trav, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *slow = head->next, *fast = head->next, *temp;
    while (1) {
        if (fast->next->next == head) {
            temp = slow->next;
            list_del(slow->next);
            q_release_element(list_entry(temp, element_t, list));
            return true;
        } else if (fast->next == head) {
            temp = slow;
            list_del(slow);
            q_release_element(list_entry(temp, element_t, list));
            return true;
        }
        slow = slow->next;
        fast = fast->next->next;
    }
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;
    struct list_head *trav, *safe;
    bool flag = false;
    list_for_each_safe(trav, safe, head) {
        if (safe != head && !strcmp(list_entry(trav, element_t, list)->value,
                                    list_entry(safe, element_t, list)->value)) {
            list_del(trav);
            q_release_element(list_entry(trav, element_t, list));
            flag = true;
        } else {
            if (flag) {
                list_del(trav);
                q_release_element(list_entry(trav, element_t, list));
                flag = false;
            }
        }
    }
    return true;
}
// auxiliary function for q_swap
void swap2node(struct list_head *node1, struct list_head *node2)
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
/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    // https://leetcode.com/problems/swap-nodes-in-pairs/

    struct list_head *trav, *safe;
    int count = 0;
    list_for_each_safe(trav, safe, head) {
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
    struct list_head *trav, *safe;
    list_for_each_safe(trav, safe, head) {
        trav->next = trav->prev;
        trav->prev = safe;
    }
    struct list_head *temp = head->next;
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
    list_for_each_safe(trav, safe, head) {
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

// Merge the two lists in one sorted list.
struct list_head *merge2sortedlist(struct list_head *left,
                                   struct list_head *right,
                                   bool descend)
{
    struct list_head *new_head = NULL, **indirect = &new_head,
                     **chosen_list_ptr = NULL;
    for (; left && right; *chosen_list_ptr = (*chosen_list_ptr)->next) {
        if (descend) {
            chosen_list_ptr =
                strcmp(list_entry(left, element_t, list)->value,
                       list_entry(right, element_t, list)->value) >= 0
                    ? &left
                    : &right;
        } else {
            chosen_list_ptr =
                strcmp(list_entry(left, element_t, list)->value,
                       list_entry(right, element_t, list)->value) <= 0
                    ? &left
                    : &right;
        }
        *indirect = *chosen_list_ptr;
        indirect = &(*indirect)->next;
    }
    *indirect = (struct list_head *) ((__int64_t) left | (__int64_t) right);
    return new_head;
}

// Divide the list into several nodes
struct list_head *merge_sort(struct list_head *L, bool descend)
{
    if (!L || L->next == NULL)
        return L;
    struct list_head *fast = L, *slow = L, *mid;
    for (; fast && fast->next; slow = slow->next, fast = fast->next->next)
        ;
    mid = slow;
    // cutoff
    slow->prev->next = NULL;
    struct list_head *left = merge_sort(L, descend);
    struct list_head *right = merge_sort(mid, descend);
    return merge2sortedlist(left, right, descend);
}


void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    // cutoff
    head->prev->next = NULL;
    head->next = merge_sort(head->next, descend);
    // link back
    struct list_head *trav = head, *safe = head->next;
    for (; safe != NULL; trav = trav->next, safe = safe->next) {
        safe->prev = trav;
    }
    trav->next = head;
    head->prev = trav;
    return;
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    struct list_head *trav = head->prev;
    struct list_head *safe;
    const char *min_so_far = list_entry(trav, element_t, list)->value;
    while (trav != head) {
        safe = trav->prev;
        if (strcmp(min_so_far, list_entry(trav, element_t, list)->value) < 0) {
            list_del(trav);
            q_release_element(list_entry(trav, element_t, list));
        } else {
            min_so_far = list_entry(trav, element_t, list)->value;
        }
        trav = safe;
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    struct list_head *trav = head->prev;
    struct list_head *safe;
    const char *max_so_far = list_entry(trav, element_t, list)->value;
    while (trav != head) {
        safe = trav->prev;
        if (strcmp(max_so_far, list_entry(trav, element_t, list)->value) > 0) {
            list_del(trav);
            q_release_element(list_entry(trav, element_t, list));
        } else {
            max_so_far = list_entry(trav, element_t, list)->value;
        }
        trav = safe;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
