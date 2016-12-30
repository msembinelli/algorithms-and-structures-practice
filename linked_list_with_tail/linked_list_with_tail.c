/*****************************************************************************
 * linked_list_with_tail.c
 * This file implements the various methods of a singly linked list with a tail
 * pointer.
 *
 * Matthew Sembinelli
 * December 24, 2016
 *****************************************************************************/

/*****************************************************************************
 *                                 Includes                                  *
 *****************************************************************************/
#include <assert.h>
#include "linked_list_with_tail.h"

/*****************************************************************************
 *                                  Types                                    *
 *****************************************************************************/
// Forward declaration of node
typedef struct node node;

// Used for iterating through the linked list to get previous and current nodes
typedef struct
{
    node* previous;
    node* current;
} nodes;

// Private node structure representation for a linked list element
struct node
{
    int key;
    node* next;
};

// Private linked list structure
typedef struct
{
    node* head;
    node* tail;
    int size;
} linked_list_private;

/*****************************************************************************
 *                             Private Prototypes                            *
 *****************************************************************************/
static nodes* get_nodes_at_index(linked_list_private* private_list, int index);

/*****************************************************************************
 *                             Public Definitions                            *
 *****************************************************************************/
linked_list* init(void)
{
    linked_list_private* list_private = (linked_list_private*) malloc(sizeof(linked_list_private));
    list_private->head = NULL;
    list_private->tail = NULL;
    list_private->size = 0;

    linked_list* list = (linked_list*) malloc(sizeof(linked_list));
    list->private = list_private;

    return list;
}

void push_front(linked_list* list, int key)
{
    linked_list_private* private_list = list->private;

    node* new_node = (node*) malloc(sizeof(node));
    new_node->key = key;
    new_node->next = private_list->head;

    private_list->head = new_node;
    private_list->size++;
}

int pop_front(linked_list* list)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    int key = private_list->head->key;
    node* delete_node = private_list->head;
    private_list->head = private_list->head->next;

    if(private_list->head == NULL)
    {
        private_list->tail = NULL;
    }

    free(delete_node);
    private_list->size--;

    return key;
}

void push_back(linked_list* list, int key)
{
    linked_list_private* private_list = list->private;

    node* new_node = (node*) malloc(sizeof(node));
    new_node->key = key;
    new_node->next = NULL;

    if(empty(list))
    {
        private_list->head = new_node;
    }
    else
    {
        private_list->tail->next = new_node;
    }
    private_list->tail = new_node;

    private_list->size++;
}

int pop_back(linked_list* list)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    nodes* previous_and_current_nodes =
            get_nodes_at_index(private_list, private_list->size - 1);
    node* previous_node = previous_and_current_nodes->previous;
    node* current_node = previous_and_current_nodes->current;

    if(private_list->head == current_node)
    {
        private_list->head = NULL;
        private_list->tail = NULL;
    }
    else
    {
        previous_node->next = NULL;
        private_list->tail = previous_node;
    }

    int key = current_node->key;
    free(current_node);
    private_list->size--;
    return key;
}

int front(linked_list* list)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    return private_list->head->key;
}

int back(linked_list* list)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    return private_list->tail->key;
}

void insert(linked_list* list, int index, int key)
{
    linked_list_private* private_list = list->private;

    // Insert at the very end of the list if desired index is >= list size
    if(index >= private_list->size)
    {
        push_back(list, key);
    }
    // Else we are inserting at an index somewhere in the middle of the list
    else
    {
        node* insert_node = (node*) malloc(sizeof(node));
        insert_node->key = key;

        nodes* previous_and_current_nodes =
                get_nodes_at_index(private_list, index);
        node* previous_node = previous_and_current_nodes->previous;
        node* current_node = previous_and_current_nodes->current;

        if(private_list->head == current_node)
        {
            private_list->head = insert_node;
            insert_node->next = current_node;
        }
        else
        {
            insert_node->next = current_node;
            previous_node->next = insert_node;
        }
        private_list->size++;
    }
}

void erase(linked_list* list, int index)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    // Erase at the very end of the list if desired index is >= list size
    if(index >= private_list->size - 1)
    {
        pop_back(list);
    }
    // Else we are erasing at an index somewhere in the middle of the list
    else
    {
        nodes* previous_and_current_nodes =
                get_nodes_at_index(private_list, index);
        node* previous_node = previous_and_current_nodes->previous;
        node* current_node = previous_and_current_nodes->current;

        if(private_list->head == current_node)
        {
            private_list->head = current_node->next;
        }
        else
        {
            previous_node->next = current_node->next;
        }

        free(current_node);
        private_list->size--;
    }
}

int value_at(linked_list* list, int index)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    return get_nodes_at_index(private_list, index)->current->key;
}

int value_n_from_end(linked_list* list, int n)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    return get_nodes_at_index(private_list,
            ((private_list->size - 1) - n))->current->key;
}

void reverse(linked_list* list)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    private_list->tail = private_list->head;

    node* previous_node = NULL;
    node* current_node = private_list->head;
    node* next_node = NULL;

    while(NULL != current_node)
    {
        next_node = current_node->next;
        current_node->next = previous_node;
        previous_node = current_node;
        current_node = next_node;
    }

    private_list->head = previous_node;
}

void remove_value(linked_list* list, int key)
{
    // Assert if the list is empty. Empty check is a precondition.
    assert(!empty(list));

    linked_list_private* private_list = list->private;

    node* current_node = private_list->head;
    node* previous_node = NULL;

    while(key != current_node->key)
    {
        if(NULL == current_node)
        {
            return;
        }
        previous_node = current_node;
        current_node = current_node->next;
    }

    previous_node->next = current_node->next;

    if(private_list->tail == current_node)
    {
        private_list->tail = previous_node;
    }

    free(current_node);
    private_list->size--;
}

int get_size(linked_list* list)
{
    linked_list_private* private_list = list->private;
    return private_list->size;
}

bool empty(linked_list* list)
{
    linked_list_private* private_list = list->private;
    return (NULL == private_list->head);
}

void destroy(linked_list* list)
{
    free(list->private);
    list->private = NULL;
    free(list);
    list = NULL;
}

// Private function definitions
static nodes* get_nodes_at_index(linked_list_private* private_list, int index)
{
    node* current = private_list->head;
    node* previous = NULL;

    for(int current_index = 0; current_index < index; current_index++)
    {
        if(NULL == current->next)
        {
            break;
        }
        previous = current;
        current = current->next;
    }

    nodes* return_nodes = (nodes*) malloc(sizeof(nodes));
    return_nodes->current = current;
    return_nodes->previous = previous;

    return return_nodes;
}
