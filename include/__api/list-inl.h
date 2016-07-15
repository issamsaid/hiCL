#ifndef __API_LIST_INL_H_
#define __API_LIST_INL_H_
///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permetted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
/// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file __api/list-inl.h
/// @author Issam SAID
/// @brief Generic-typed linked list.
///
typedef char* string;

#define DEFINE_ENTRY_TYPE(type)     \
    typedef struct __list_##type {  \
        struct __list_##type *next; \
        type data;                  \
    } list_##type;

#define DEFINE_CREATE_ENTRY(type)   \
    list_##type* list_create_##type(type data);
#define DEFINE_INSERT_ENTRY(type)   \
    void list_insert_##type(list_##type **head, list_##type* entry);
#define DEFINE_REMOVE_ENTRY(type)   \
    list_##type* list_remove_##type(list_##type **head, type data);
#define DEFINE_FIND_ENTRY(type)     \
    list_##type* list_find_##type(list_##type **head, type data);
#define DEFINE_DELETE_ENTRY(type)   \
    void list_delete_##type(list_##type** entry);
#define DEFINE_LIST_SIZE(type)   \
    size_t list_size_##type(list_##type** head);

#define GENERATE_LIST_HEADER(type) \
    DEFINE_ENTRY_TYPE(type)        \
    DEFINE_CREATE_ENTRY(type)      \
    DEFINE_INSERT_ENTRY(type)      \
    DEFINE_REMOVE_ENTRY(type)      \
    DEFINE_FIND_ENTRY(type)        \
    DEFINE_DELETE_ENTRY(type)      \
    DEFINE_LIST_SIZE(type)        

#define IMPLEMENT_CREATE_ENTRY(type)                                     \
    list_##type* list_create_##type(type data) {                         \
        list_##type* entry = (list_##type*) malloc(sizeof(list_##type)); \
        entry->data = data;                                              \
        entry->next = NULL;                                              \
        return entry;                                                    \
    }

#define IMPLEMENT_INSERT_ENTRY(type)                                     \
    void list_insert_##type(list_##type** head, list_##type* entry) {    \
        list_##type*  n    = *head;                                      \
        list_##type** prev =  head;                                      \
        while(n != NULL) {                                               \
            prev = &n->next;                                             \
            n    = n->next;                                              \
        }                                                                \
        *prev = entry;                                                   \
    }

#define IMPLEMENT_REMOVE_ENTRY(type)                                     \
    list_##type* list_remove_##type(list_##type** head, type data) {     \
        list_##type* to_remove;                                          \
        list_##type*  n    = *head;                                      \
        list_##type** prev =  head;                                      \
        while(n != NULL) {                                               \
            if (n->data == data) {                                       \
                to_remove = n;                                           \
                *prev     = n->next;                                     \
            }                                                            \
            prev = &n->next;                                             \
            n    = n->next;                                              \
        }                                                                \
        return to_remove;                                                \
    }

#define IMPLEMENT_FIND_ENTRY(type)                                       \
    list_##type* list_find_##type(list_##type** head, type data) {       \
        list_##type* n = *head;                                          \
        while(n != NULL) {                                               \
            if (n->data == data) break;                                  \
            n=n->next;                                                   \
        }                                                                \
        return n;                                                        \
    }

#define IMPLEMENT_DELETE_ENTRY(type)                                     \
    void list_delete_##type(list_##type** entry) {                       \
        (*entry)->next = NULL;                                           \
        free(*entry);                                                    \
        *entry = NULL;                                                   \
    }

#define IMPLEMENT_LIST_SIZE(type)                                        \
    size_t list_size_##type(list_##type** head) {                        \
        size_t count = 0;                                                \
        list_##type* n = *head;                                          \
        while(n != NULL) {                                               \
            count++;                                                     \
            n=n->next;                                                   \
        }                                                                \
        return count;                                                    \
    }

#define GENERATE_LIST_BODY(type)  \
    IMPLEMENT_CREATE_ENTRY(type)  \
    IMPLEMENT_INSERT_ENTRY(type)  \
    IMPLEMENT_REMOVE_ENTRY(type)  \
    IMPLEMENT_FIND_ENTRY(type)    \
    IMPLEMENT_DELETE_ENTRY(type)  \
    IMPLEMENT_LIST_SIZE(type)

#endif  // __API_LIST_INL_H_