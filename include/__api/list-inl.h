#ifndef __API_LIST_INL_H_
#define __API_LIST_INL_H_
///
/// @copyright Copyright 2013 - 2014 Total S.A. All rights reserved.
///
/// NOTICE: All information contained herein is, and remains
/// the property of Total S.A. and its suppliers, if any.
/// The intellectual and technical concepts contained herein
/// are proprietary to Total S.A. and its suppliers and may be
/// covered by U.S. and Foreign Patents, patents in process,
/// and are protected by trade secret or copyright law.
/// Dissemination of this information or reproduction of this
/// material is strictly forbidden unless prior written permission
/// is obtained from Total S.A.
/// 
/// @author Issam Said
/// @file list-inl.h
/// @version $Id$
/// @brief generic list structure.
///
#include "__api/config/guard.h"

CPPGUARD_BEGIN()

typedef char* string;

#define DEFINE_NODE(type)          \
    typedef struct _node_##type {  \
        struct _node_##type *next; \
        type data;                 \
    } node_##type;

#define DEFINE_NODE_RELEASE_FUNCTION(type)      \
    typedef void (*fn_cleaner_node_##type)(type);

#define DEFINE_LIST(type)               \
    typedef struct _list_##type {       \
        node_##type *head;              \
        fn_cleaner_node_##type cleaner; \
        size_t size;                    \
    } list_##type;

#define DEFINE_CREATE_LIST(type) \
    void create_list_##type(list_##type *list, fn_cleaner_node_##type fn);
#define DEFINE_DELETE_LIST(type) \
    void delete_list_##type(list_##type *list);
#define DEFINE_RESET_LIST(type) \
    void reset_list_##type(list_##type *list);
#define DEFINE_CREATE_NODE(type) \
    node_##type *create_node_##type(type data);
#define DEFINE_DELETE_NODE(type) \
    void delete_node_##type(node_##type *node);
#define DEFINE_INSERT_NODE(type) \
    void insert_node_##type(list_##type *list, type data);
#define DEFINE_ADD_NODE(type) \
    void add_node_##type(list_##type *list, node_##type *node);
#define DEFINE_SKIP_NODE(type) \
    void skip_node_##type(list_##type *list, type data);
#define DEFINE_REMOVE_NODE(type) \
    void remove_node_##type(list_##type *list, type data);
#define DEFINE_FIND_NODE(type) \
    node_##type *find_node_##type(list_##type *list, type data);
#define DEFINE_GET_NODE_DATA(type) \
    type get_##type(list_##type *list, size_t idx);

#define GENERATE_LIST_HEADER(type)     \
    DEFINE_NODE(type)                  \
    DEFINE_NODE_RELEASE_FUNCTION(type) \
    DEFINE_LIST(type)                  \
    DEFINE_CREATE_LIST(type)           \
    DEFINE_DELETE_LIST(type)           \
    DEFINE_RESET_LIST(type)            \
    DEFINE_CREATE_NODE(type)           \
    DEFINE_DELETE_NODE(type)           \
    DEFINE_INSERT_NODE(type)           \
    DEFINE_ADD_NODE(type)              \
    DEFINE_REMOVE_NODE(type)           \
    DEFINE_FIND_NODE(type)             \
    DEFINE_GET_NODE_DATA(type)

#define IMPLEMENT_CREATE_LIST(type)                                         \
    void create_list_##type(list_##type *list, fn_cleaner_node_##type fn) { \
        list->head    = NULL;                                               \
        list->size    = 0;                                                  \
        list->cleaner = fn;                                                 \
    }

#define IMPLEMENT_DELETE_LIST(type)                                     \
    void delete_list_##type(list_##type *list) {                        \
        node_##type *tmp, *n=list->head;                                \
        while(n != NULL) {                                              \
            tmp = n->next;                                              \
            if (list->cleaner) list->cleaner(n->data);                  \
            delete_node_##type(n);                                      \
            list->size--;                                               \
            n = tmp;                                                    \
        }                                                               \
        list = NULL;                                                    \
    }

#define IMPLEMENT_RESET_LIST(type)                                      \
    void reset_list_##type(list_##type *list) {                         \
        node_##type *tmp, *n=list->head;                                \
        while(n != NULL) {                                              \
            tmp = n->next;                                              \
            delete_node_##type(n);                                      \
            list->size--;                                               \
            n = tmp;                                                    \
        }                                                               \
    }

#define IMPLEMENT_CREATE_NODE(type)                                      \
    node_##type *create_node_##type(type data) {                         \
        node_##type * node = (node_##type*) malloc(sizeof(node_##type)); \
        node->data = data;                                               \
        node->next = NULL;                                               \
        return node;                                                     \
    }

#define IMPLEMENT_DELETE_NODE(type)                      \
    void delete_node_##type(node_##type *node) {         \
        free(node);                                      \
        node = NULL;                                     \
    }


#define IMPLEMENT_INSERT_NODE(type)                         \
    void insert_node_##type(list_##type *list, type data) { \
        node_##type *n, *node = create_node_##type(data);   \
        if (list->size == 0) {                              \
            list->head = node;                              \
        } else {                                            \
            n = list->head;                                 \
            while(n->next != NULL) n=n->next;               \
            n->next = node;                                 \
        }                                                   \
        list->size++;                                       \
    }

#define IMPLEMENT_ADD_NODE(type)                                 \
    void add_node_##type(list_##type *list, node_##type *node) { \
        node_##type *n;                                          \
        if (list->size == 0) {                                   \
            list->head = node;                                   \
        } else {                                                 \
            n = list->head;                                      \
            while(n->next != NULL) n=n->next;                    \
            n->next = node;                                      \
        }                                                        \
        list->size++;                                            \
    }

#define IMPLEMENT_REMOVE_NODE(type)                               \
    void remove_node_##type(list_##type *list, type data) {       \
        node_##type *tmp, *n = list->head;                        \
        if (list->size == 0) return;                              \
        if (list->head->data == data) {                           \
            tmp = list->head->next;                               \
            if (list->cleaner)                                    \
                list->cleaner(list->head->data);                  \
            delete_node_##type(list->head);                       \
            list->size--;                                         \
            list->head = tmp;                                     \
        } else {                                                  \
            while(n->next != NULL) {                              \
                if (n->next->data == data) {                      \
                    tmp     = n->next;                            \
                    n->next = n->next->next;                      \
                    if (list->cleaner)                            \
                        list->cleaner(tmp->data);                 \
                    delete_node_##type(tmp);                      \
                    list->size--;                                 \
                } else {                                          \
                    n = n->next;                                  \
                }                                                 \
            }                                                     \
        }                                                         \
    }

#define IMPLEMENT_SKIP_NODE(type)                               \
    void skip_node_##type(list_##type *list, type data) {       \
        node_##type *tmp, *n = list->head;                        \
        if (list->size == 0) return;                              \
        if (list->head->data == data) {                           \
            tmp = list->head->next;                               \
            delete_node_##type(list->head);                       \
            list->size--;                                         \
            list->head = tmp;                                     \
        } else {                                                  \
            while(n->next != NULL) {                              \
                if (n->next->data == data) {                      \
                    tmp     = n->next;                            \
                    n->next = n->next->next;                      \
                    delete_node_##type(tmp);                      \
                    list->size--;                                 \
                } else {                                          \
                    n = n->next;                                  \
                }                                                 \
            }                                                     \
        }                                                         \
    }

#define IMPLEMENT_FIND_NODE(type)                                 \
    node_##type *find_node_##type(list_##type *list, type data) { \
        node_##type *n = list->head;                              \
        while(n != NULL) {                                        \
            if (n->data == data) break;                           \
            n=n->next;                                            \
        }                                                         \
        return n;                                                 \
    }

#define IMPLEMENT_GET_NODE_DATA(type)                               \
    type get_##type(list_##type *list, size_t idx) {                \
        size_t i;                                                   \
        node_##type *n;                                             \
        if (idx > list->size) {                                     \
            fprintf(stderr, "list error: index out of bound\n");    \
            exit(EXIT_FAILURE);                                     \
        } else {                                                    \
            for (n=list->head, i=0; i!=idx; n=n->next, ++i);        \
            return n->data;                                         \
        }                                                           \
    }

#define GENERATE_LIST_BODY(type)   \
    IMPLEMENT_CREATE_LIST(type)    \
    IMPLEMENT_DELETE_LIST(type)    \
    IMPLEMENT_RESET_LIST(type)     \
    IMPLEMENT_CREATE_NODE(type)    \
    IMPLEMENT_DELETE_NODE(type)    \
    IMPLEMENT_INSERT_NODE(type)    \
    IMPLEMENT_ADD_NODE(type)       \
    IMPLEMENT_REMOVE_NODE(type)    \
    IMPLEMENT_SKIP_NODE(type)      \
    IMPLEMENT_GET_NODE_DATA(type)  \
    IMPLEMENT_FIND_NODE(type)

CPPGUARD_END()

#endif  // __API_LIST_INL_H_